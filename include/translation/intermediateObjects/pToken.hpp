#ifndef P_TOK_HPP
#   define P_TOK_HPP

#include <string>

namespace photon {

    /** Represents types of tokens */
    enum class TokenType {
        // Literals
        IDENTIFIER,         // A name in the code
        STRING,             // Text between "" markers
        CHAR,               // A character between '' markers
        
        // Subcat : NUMBERS
        INT,                // Integer
        INT_BIN_FORMAT,     // 0b formated integers
        INT_OCT_FORMAT,     // 0o formated integers
        INT_HEX_FORMAT,     // 0x formated integers

        FLOAT,              // Floating point
        FLOAT_HEX_FORMAT,   // 0x formatted floats

        // Structural
        KEYWORD,            // A specific instruction
        SYMBOL,             // A symbol in the code

        // Delimitations
        END_FILE,           // EOF

        // Misc
        INVALID             // An invalid token
    };

    /** Holds informations about extracted elements */
    struct Token {
        /// @brief The token type, defined by the enum @c TokenType
        TokenType type;
        
        /// @brief The string value of the token
        std::string lexeme;
    };

    /** Asserts that the token's type belongs to a numeric */
    inline bool isNum(Token& tok) {
        return (
            tok.type == TokenType::INT            ||
            tok.type == TokenType::INT_BIN_FORMAT ||
            tok.type == TokenType::INT_OCT_FORMAT ||
            tok.type == TokenType::INT_HEX_FORMAT ||
            tok.type == TokenType::FLOAT          ||
            tok.type == TokenType::FLOAT_HEX_FORMAT
        );
    }

} // namespace photon


#endif // P_TOKEN_HPP