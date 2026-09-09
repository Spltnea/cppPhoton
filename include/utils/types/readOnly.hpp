#ifndef READ_ONLY_VAR_HPP
#   define READ_ONLY_VAR_HPP

#include <ostream>

namespace photon {

    /** Manages read only attributes, directly copied from https://github.com/illescasDaniel/Var-ReadOnly/tree/master
     *  @tparam T The variable type
     *  @tparam FriendClass The class that can modify this variable (usualy the class where the variable is a member of)
     */
    template<typename T, typename FriendClass>
    class readOnly_t {
        T val {};

        friend FriendClass;

        inline readOnly_t() {}

        template<typename any>
        inline readOnly_t(const any& newVal) { val = newVal; }
        
        inline friend std::ostream& operator<<(std::ostream& os, const readOnly_t& variable) { return os << variable.val; }

    public :
        inline operator T() const { return val; }
    }; // class readOnly_t
}

#endif