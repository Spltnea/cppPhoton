#ifndef P_LEXER_HPP
#   define P_LEXER_HPP

#include "pToken.hpp"
#include "pPreprocessor.hpp"
#include "utils/types/commonVectors.hpp"

namespace photon {

    /** Transforms a source code into exploitable tokens */
    class pLexer {
    private :
        // == Private Members ==

        // The array of processed tokens
        tokenVec_t tokens;

        // The char array containing the processed text file
        charVec_t buffer;

        // The path to the processed photon script file
        std::string ppfPath;

        charVec_t::const_iterator index;
        
        // == Helpers ==

        // Checks if the target offset is valid relative to buffer limits, returns false if the buffer is empty
        bool isValidOffset(int offset) const {
            if (buffer.empty()) return false;

            // Absolute target index calculation
            auto currentIndex = std::distance(buffer.begin(), index);
            auto targetIndex = currentIndex + offset;

            return targetIndex >= 0 && targetIndex < static_cast<std::ptrdiff_t>(buffer.size());
        }

        // Asserts that the cursor has reached the end of stream
        bool isAtEnd(void) const {
            return index == buffer.end();
        }

        // Asserts that the given character is a new line
        bool isNewLine(char& c) {
            return (c == '\n');
        }

        // Asserts that the given character is a valid whitespace character (' ', \n, \f, \r, \t and \v)
        bool isWhitespace(char& c) {
            return std::isspace(static_cast<unsigned char>(c));
        }

        // Returns the current element without advancing the index cursor
        char currentElement(void) const {
            return *index;
        }

        // Returns the current element and advances the index cursor
        char returnAndAdvance(void) {
            char current = currentElement();
            std::advance(index, 1);
            return current;
        }

        // Returns the next element without advancing the index cursor
        char nextElement(void) const {
            return *(index + 1);
        }

        // Returns an element in stream relative to an offset, does not advances the index cursor
        // Returns a log frame and the null character if the position is invalid
        char elementAt(int offset) const {
            if (isValidOffset(offset)) {
                return *(index + offset);
            } else {
                auto logFrame = pLogger::buildFrame(
                    IdPrefix::LEXER_LOG, 
                    SeverityPrefix::ERR, 0x03, 
                    {"Error parsing the char vector, the target index goes out of bounds, returning '\\0'"}
                );
                pLogger::lprint(logFrame);
                return '\0';
            }
        }

    public :
        /**
         * Creates a new instance of the pLexer class, clears both char and tokens arrays for newer files
         * @param prepResult The result of the preprocessing pass to extract the processed file name & path
         */
        explicit pLexer(const PreprocessResult& prepResult) : ppfPath(prepResult.processedFilePath) {
            buffer.clear();
            tokens.clear();

            std::ifstream fileToProcess(ppfPath, std::ios::binary | std::ios::ate);
            if (!fileToProcess.is_open()) {
                auto logFrame = pLogger::buildFrame(IdPrefix::LEXER_LOG, 
                                                        SeverityPrefix::ERR, 0x01, 
                                                        {"Cannot open file : \"", ppfPath, "\" as file does not exist at specified location\n", 
                                                         "Note that the char buffer will stay empty and will not be suitable for processing"
                                                        });
                pLogger::lprint(logFrame);
                return;
            }

            std::streamsize stsize = fileToProcess.tellg();
            fileToProcess.seekg(0, std::ios::beg);

            buffer.resize(stsize);

            if (stsize > 0) {
                fileToProcess.read(buffer.data(), stsize);
            }

            index = buffer.begin();
        }

        int lineNo = 1;
        int colNo = 1;

        // == Getters

        /** Returns the token list as an immutable reference */
        const tokenVec_t& getTokens(void) const {
            return tokens;
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