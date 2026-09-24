#ifndef PHOTON_RESERVED_KEYWORDS_HPP
#   define PHOTON_RESERVED_KEYWORDS_HPP

#include <string>
#include <vector>

namespace photon {

    // A vector of reserved keywords
    const std::vector<std::string> reservedKeywords = {

        // Variables
        "var", "const", "ptr"

        // Type declarations
        "i8", "i16", "i32", "i64", "i128",
        "u8", "u16", "u32", "u64", "u128",
        "f8", "f16", "f32", "f64", "f128",
        "bool",
        "char",
        "void",

        // Builtin Literals
        "true", "false",
        "nullval", "nullptr",

        // Conditional
        "if", "else",
        
        // Looping
        "repeat", "while", "until", "forever",

        // Flow Control
        "pass", "break", "continue", "skip", "return",

        // Declarations
        "func", "struct", "enum",
    };

    // A vector of symbols
    const std::vector<char> symbols = {
        // Pairs
        '{', '}',
        '(', ')',
        '[', ']',

        // Reserved symbols
        ',', ';', ':', '@', '#',

        // Others
        '+', '-', '/', '*', '%', '=', '<', '>', '&', '!', '~', '|', '?',
    };

    // A vector of symbols that can be combined
    const std::vector<char> combinableSymbols = {
        '+', '-', '/', '*', '%', '=', '<', '>', '&', '!', '~', '|', '?',
    };

} // namespace photon


#endif