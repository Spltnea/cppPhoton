#ifndef P_LEXER_HPP
#   define P_LEXER_HPP

#include "intermediateObjects/pToken.hpp"
#include "pPreprocessor.hpp"

#include "utils/types/iterableBuffer.hpp"
#include "utils/vectorUtilities.hpp"
#include "helpers/reservedEntries.hpp"
#include "helpers/regexRules.hpp"

#include <iostream>

namespace photon {

    /** Transforms a source code into exploitable tokens */
    class pLexer {
    private :
        // The array of processed tokens
        iterableBuffer<Token> tokenBuffer;
        
        /**
         * Asserts that the given character is a valid whitespace character (' ', \\n, \f, \r, \t and \v)
         * @param c The given character
        */
        bool isWhitespace(const char C);

        /**
         * Asserts that the given character is a letter
         * @param c The given character
         */
        bool isLetter(const char C);

        /**
         * Asserts that the given character is a symbol
         */
        bool isSymbol(const char C);

        /**
         * Reads a word starting from the first character until the next whitespace or symbol and returns a token
         * @param BEGIN The first character to begin from
         * @return A keyword token if the lexeme belongs to the keyword list, an identifier otherwise
         */
        Token readWord(iterableBuffer<char>& charBuffer, const char BEGIN);

        /**
         * Reads a symbol from the first character until the next non symbol character and returns a token
         * @param BEGIN The first character to begin from
         */
        Token readSymbol(iterableBuffer<char>& charBuffer, const char BEGIN);

        /**
         * Reads a number from the first character until the next non number character and returns a token
         */
        Token readNumber(iterableBuffer<char>& charBuffer);

         /**
         * Reads a string value or char value between two matching delimiters
         * @param DELIMITER The symbol that delimits the value
         */
        Token readStringOrChar(iterableBuffer<char>& charBuffer, const char DELIMITER);

    public :
        /** Gets the lexer's token list */
        iterableBuffer<Token>& getTokens() {
            return tokenBuffer;
        }

        /** Processes a char buffer into a stream of tokens
         * Note : The token stream will ALWAYS contain the following token :
         * - END_OF_FILE token
         * 
         * If anything goes wrong during the lexing pass, this token only will remain inside the token stream
         * To get the processed stream, use the provided getTokens() function
        */
        void applyLexerPass(iterableBuffer<char>& charBuffer);

    }; // class pLexer
} // namespace photon

#endif // P_LEXER_HPP