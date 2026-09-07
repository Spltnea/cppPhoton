#ifndef P_LEXER_HPP
#   define P_LEXER_HPP

#include "pToken.hpp"
#include "pPreprocessor.hpp"
#include "utils/pLogger.hpp"

namespace photon {

    /** Transforms a source code into exploitable tokens */
    class pLexer {
    private :
        tokenArray_t tokens;

        std::string ppfPath;
        int lineNo = 1;
        int colNo = 1;
        
        // == TODO : HELPERS ==

    public :
        

        explicit pLexer(const PreprocessResult& prepResult) : ppfPath(prepResult.processedFilePath) {}
        explicit pLexer(std::string _ppfPath) : ppfPath(std::move(_ppfPath)) {}

        /** Transforms the cleaned source file into a stream of tokens 
         * Note : The token stream will ALWAYS contain the following token :
         * - END_OF_FILE token
         * 
         * If anything goes wrong during the lexing pass, this token only will remain at coords {1; 1} inside the token stream
        */
        void applyLexerPass() {

            std::ifstream processedFile(ppfPath);
            if (!processedFile.is_open()) {
                auto logFrame = pLogger::buildFrame(IdPrefix::LEXER_LOG, 
                                                    SeverityPrefix::ERR, 0x01, 
                                                    {"Cannot open file : \"", ppfPath, "\" as file does not exist at specified location"});
                pLogger::lprint(logFrame);

                tokens.push_back({1, 1, TokenType::END_FILE, "EOF"});
                return;
            }

            // == TODO : PROCESS LOOP HERE ==

            tokens.push_back({lineNo, colNo, TokenType::END_FILE, "EOF"});
        }

        /** Returns the token list as an immutable reference */
        const tokenArray_t& getTokens() const {
            return tokens;
        }
    };
}

#endif