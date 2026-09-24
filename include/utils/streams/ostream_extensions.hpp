#ifndef OSTREAM_EXT_HPP
#   define OSTREAM_EXT_HPP

#include <iostream>
#include <ostream>

#include "translation/intermediateObjects/pToken.hpp"

namespace photon {
    inline std::ostream& operator <<(std::ostream& os, const Token& t) {
        os << "[Token Type : " << static_cast<int>(t.type) << "] | [Token Lexeme : " << t.lexeme << "]\n";
        return os;
    }
}

#endif