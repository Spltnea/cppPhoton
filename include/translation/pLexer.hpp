#ifndef P_LEXER_HPP
#   define P_LEXER_HPP

#include "pToken.hpp"
#include "pPreprocessor.hpp"

#include "utils/types/iterableBuffer.hpp"
#include "utils/vectorUtilities.hpp"
#include "reservedEntries.hpp"
#include "regexRules.hpp"

#include <iostream>

namespace photon {

    /** Transforms a source code into exploitable tokens */
    class pLexer {
    private :
        // == Private Members ==

        // The array of processed tokens
        iterableBuffer<Token> tokenBuffer;

        // The char array containing the processed text file
        iterableBuffer<char> charBuffer;

        // The path to the processed photon script file
        std::string ppfPath;
        
        // == Helpers ==

        /**
         * Asserts that the given character is a new line)
         * @param c The given character
        */
        bool isNewLine(const char c) {
            return (c == '\n');
        }

        /**
         * Asserts that the given character is a valid whitespace character (' ', \\n, \f, \r, \t and \v)
         * @param c The given character
        */
        bool isWhitespace(const char c) {
            return std::isspace(static_cast<unsigned char>(c));
        }

        /**
         * Asserts that the given character is a pure whitespace (horizontal tab or WS only)
         */
        bool isPureWhitespace(const char c) {
            return std::isblank(static_cast<unsigned char>(c));
        }

        /**
         * Asserts that the given character is the same as the expected character
         * @param c The given character
         * @param expectation The character to expect
         */
        bool compare(const char c, char expectation) {
            return (c == expectation);
        }

        /**
         * Asserts that the given character is a letter
         * @param c The given character
         */
        bool isLetter(const char c) {
            return std::isalpha(c);
        }

        /**
         * Asserts that the given character is a symbol
         */
        bool isSymbol(const char c) {
            return (isItemPresentIn(c, symbols));
        }

        // == Processing Helpers ==

        /**
         * Reads a word starting from the first character until the next whitespace or symbol and returns a token
         * @param begin The first character to begin from
         * @return A keyword token if the lexeme belongs to the keyword list, an identifier otherwise
         */
        Token readWord(char begin) {
            std::string lexeme;
            char c = begin;
            lexeme += c;
            
            while (!std::isspace(static_cast<unsigned char>(charBuffer.currentElement())) && !isSymbol(charBuffer.currentElement())) {  
                lexeme += charBuffer.advance();
            }

            // Checks if the lexeme refers to a known keyword
            if (isItemRefPresentIn(lexeme, reservedKeywords)) {
                return {TokenType::KEYWORD, lexeme};
            }

            return {TokenType::IDENTIFIER, lexeme};
        }

        /**
         * Reads a symbol from the first character until the next non symbol character and returns a token
         * @param begin The first character to begin from
         */
        Token readSymbol(char& begin) {
            std::string lexeme;
            char c = begin;
            lexeme += c;

            
            while (isSymbol(charBuffer.currentElement())) {
                if (isItemPresentIn(charBuffer.currentElement(), combinableSymbols)) {
                    lexeme += charBuffer.advance();
                } else {
                    break;
                }
            }

            return {TokenType::SYMBOL, lexeme};
        }

        /**
         * Reads a number from the first character until the next non number character and returns a token
         */
        Token readNumber() {
            size_t startPos = charBuffer.position() - 1;

            auto current_it = charBuffer.begin() + startPos;
            auto end_it = charBuffer.end();

            std::match_results<decltype(current_it)> match;

            for (const auto& rule : NUMBER_RULES) {
                if (std::regex_search(current_it, end_it, match, rule.second, std::regex_constants::match_continuous)) {
                    std::string lexeme = match.str();

                    size_t charsLeft = match.length() - 1;
                    for (size_t i = 0; i < charsLeft; ++i) {
                        charBuffer.advance();
                    }

                    return { rule.first, lexeme };
                }
            }

            return { TokenType::INVALID, "invalid" };
        }

         /**
         * Reads a string value or char value between two matching delimiters
         * @param delimiter The symbol that delimits the value
         */
        Token readStringOrChar(char delimiter) {
            std::string lexeme = "";
            
            while (charBuffer.currentElement() != delimiter) {
                if (charBuffer.isAtEnd()) {
                    return { TokenType::INVALID, "invalid" };
                }

                if (charBuffer.currentElement() == '\\') {
                    charBuffer.advance();
                    
                    if (charBuffer.isAtEnd()) {
                        return { TokenType::INVALID, "invalid" };
                    }
                    
                    char escaped = charBuffer.advance();

                    switch (escaped) {
                        case 'n':  lexeme += '\n'; break; 
                        case 't':  lexeme += '\t'; break; 
                        case 'r':  lexeme += '\r'; break; 
                        case '"':  lexeme += '"';  break; 
                        case '\'': lexeme += '\''; break; 
                        case '\\': lexeme += '\\'; break; 
                        default: 
                            lexeme += '\\';
                            lexeme += escaped;
                            break;
                    }
                    continue;
                }

                lexeme += charBuffer.advance();
            }

            charBuffer.advance(); 

            if (delimiter == '\'') {
                return { TokenType::CHAR, lexeme };
            } else {
                return { TokenType::STRING, lexeme };
            }
        }

    public :
        /**
         * Creates a new instance of the pLexer class, clears both char and tokens arrays for newer files
         * @param prepResult The result of the preprocessing pass to extract the processed file name & path
         */
        explicit pLexer(const PreprocessResult& prepResult) : ppfPath(prepResult.processedFilePath) {
            charBuffer.resetBuffer();
            tokenBuffer.resetBuffer();

            std::ifstream fileToProcess(ppfPath, std::ios::binary | std::ios::ate);
            if (!fileToProcess.is_open()) {
                auto logFrame = pLogger::buildFrame
                (
                    IdPrefix::LEXER_LOG, 
                    SeverityPrefix::ERR, 0x01, 
                    {
                        "Cannot open file : \"", ppfPath, "\" as file does not exist at specified location\n", 
                        "Note that the char buffer will stay empty and will not be suitable for processing"
                    }
                );
                pLogger::lprint(logFrame);
                return;
            }

            std::streamsize stsize = fileToProcess.tellg();
            fileToProcess.seekg(0, std::ios::beg);

            if (stsize > 0) {
                std::vector<char> buf(stsize);
                fileToProcess.read(buf.data(), stsize);
                charBuffer.setBuffer(std::move(buf));
            }
        }

        // == Getters

        /** Returns the token list as an immutable reference */
        const iterableBuffer<Token>& getTokens(void) const {
            return tokenBuffer;
        }

        // == Main Processing

        /** Transforms the cleaned source file into a stream of tokens 
         * Note : The token stream will ALWAYS contain the following token :
         * - END_OF_FILE token
         * 
         * If anything goes wrong during the lexing pass, this token only will remain at coords {1; 1} inside the token stream
         * To get the processed stream, use the provided getTokens() function
        */
        void applyLexerPass(void);

    }; // class pLexer
} // namespace photon

#endif // P_LEXER_HPP