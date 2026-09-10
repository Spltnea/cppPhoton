#ifndef PHOTON_RESERVED_KEYWORDS_HPP
#   define PHOTON_RESERVED_KEYWORDS_HPP

#include <string>
#include <vector>
#include <algorithm>

namespace photon {

    // A vector of reserved keywords
    const std::vector<std::string> reservedKeywords = {
        // Declarations
        "entrypoint"
    };

    // A vector of symbols
    const std::vector<char> symbols = {
        // Coupled symbols
        '{', '}',
        '(', ')',

        // Operators
        ':',

        // Markers
        ';',
    };

} // namespace photon


#endif