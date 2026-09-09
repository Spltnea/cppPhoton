#ifndef P_LEXER_HPP
#   define P_LEXER_HPP

#include "pToken.hpp"
#include "pPreprocessor.hpp"
#include "utils/types/iterableBuffer.hpp"

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

        // Asserts that the given character is a new line
        bool isNewLine(char& c) {
            return (c == '\n');
        }

        // Asserts that the given character is a valid whitespace character (' ', \n, \f, \r, \t and \v)
        bool isWhitespace(char& c) {
            return std::isspace(static_cast<unsigned char>(c));
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

            if (stsize > 0) {
                std::vector<char> buf(stsize);
                fileToProcess.read(buf.data(), stsize);
                charBuffer.setBuffer(std::move(buf));
            }
        }

        int lineNo = 1;
        int colNo = 1;

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