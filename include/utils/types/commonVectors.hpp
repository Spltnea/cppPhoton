// Shortcuts for heavily used vectors in this project

#ifndef COMMON_VECTORS_HPP
#   define COMMON_VECTORS_HPP

#include <vector>
#include "translation/pToken.hpp"

namespace photon {
    // A vector of chars
    using charVec_t = std::vector<char>;

    // A vector of tokens
    using tokenVec_t = std::vector<Token>;
}

#endif