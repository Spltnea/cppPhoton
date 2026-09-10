#ifndef VEC_UTILS_HPP
#   define VEC_UTILS_HPP

#include <vector>
#include <algorithm>

namespace photon {
    /** 
     * Asserts that an element is present in a vector (pass by reference)
     * @param item The item to look for
     * @param vec The vector to check into
    */
    template<typename T>
    bool isItemRefPresentIn(T& item, const std::vector<T>& vec) {
        return (std::find(vec.begin(), vec.end(), item) != vec.end());
    }

    /** 
     * Asserts that an element is present in a vector
     * @param item The item to look for
     * @param vec The vector to check into
    */
    template<typename T>
    bool isItemPresentIn(T item, const std::vector<T>& vec) {
        return (std::find(vec.begin(), vec.end(), item) != vec.end());
    }
}

#endif