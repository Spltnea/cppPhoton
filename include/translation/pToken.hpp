#ifndef P_TOK_HPP
#   define P_TOK_HPP

#include <string>

namespace photon {

    /** Represents type of tokens */
    enum class TokenType {
        // Literals
        IDENTIFIER,
        STRING,
        CHAR,
        NUMBER,

        // Structural
        KEYWORD,
        SYMBOL,

        // Delimitations
        END_FILE
    };

    /** Holds informations about extracted elements */
    struct Token {
        int lineNo, colNo;
        TokenType type;
        std::string lexeme;
    };
} // namespace photon


#endif // P_TOKEN_HPP