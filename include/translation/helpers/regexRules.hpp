#ifndef PHOTON_REGEX_RULES_HPP
#   define PHOTON_REGEX_RULES_HPP

#include <string>
#include <regex>
#include <vector>
#include "pToken.hpp"

namespace photon {
    const std::vector<std::pair<TokenType, std::regex>> NUMBER_RULES = {

        // Hexadecimal Float (Matches 0x1.5p3, -0x.Ap-2)
        {TokenType::FLOAT_HEX_FORMAT, std::regex("^0[xX](?:[0-9a-fA-F]+\\.?[0-9a-fA-F]*|\\.[0-9a-fA-F]+)[pP][+-]?[0-9]+")},

        // Hexadecimal Integer (Matches 0xFF, 0X1A)
        {TokenType::INT_HEX_FORMAT,   std::regex("^0[xX][0-9a-fA-F]+")},
        
        // Binary Integer (Matches 0b1101)
        {TokenType::INT_BIN_FORMAT,   std::regex("^0[bB][0-1]+")},
        
        // Octal Integer (Matches 0o77)
        {TokenType::INT_OCT_FORMAT,   std::regex("^0[oO][0-7]+")},
        
        // Decimal Float (Matches 0.123, .45, 1e5)
        {TokenType::FLOAT,            std::regex("^(?:[0-9]+\\.[0-9]*|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?|^[0-9]+[eE][+-]?[0-9]+")},
        
        // Decimal Integer (Matches 12345)
        {TokenType::INT,              std::regex("^[0-9]+")}

    };
}


#endif