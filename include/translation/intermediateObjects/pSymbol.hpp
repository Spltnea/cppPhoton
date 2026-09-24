#ifndef P_SYMBOL_HPP
#   define P_SYMBOL_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <stdint.h>

namespace photon {

    /** A toolbox for making symbols with ease */
    class Symbols {
    public:
        /** A generic symbol */
        class BaseSymbol {
        public:
            // Contains all the avaiable symbol types
            enum class SymbolType { PACKAGE, TYPE, RECORD, FNSET, STRUCTURE, INTERFACE, FUNCTION, OPERATOR, VARIABLE, CONSTANT };

            // Contains all the avaible attributs modifiers for symbols, may be applicable or not
            enum class Attributes : uint16_t {
                NO_ATTR    = 0,             // NO ATTRS

                // Generic Attributes
                UTILITY_ATTR   = 1 << 0,    // The "utility" attribute, makes a function member of a struct not require an object instance to be used
                INLINE_ATTR    = 1 << 1,    // The "inline" attribute, instead of a classic function call, makes the compiler expand the code on site
                ABSTRACT_ATTR  = 1 << 2,    // The "abstract" attribute, the function has the option to be override (if there is a default implementation), or implemented
                EXTERN_ATTR    = 1 << 3,    // The "extern" attribute, the member is defined else where in an another language by FFI or the standard C ABI
                EXPORT_ATTR    = 1 << 4,    // The "export" attribute, the member is exported and can be used by FFI (eg in C# : "static extern, static partial")

                // Structs only
                PACKED_ATTR    = 1 << 5,    // The "packed" attribute, tells the compiler to not add padding bytes to the struct in order for it to be memory aligned

                // Variables
                SHARED_ATTR    = 1 << 6,    // The "shared" attribute, this variable's value is shared among all the struct instances

                // Visibility Modifiers
                GLOBAL_ATTR    = 1 << 7,    // The "global" attribute, the field, struct or such belongs to the global namespace and is visible everywhere without importation
                PUBLIC_ATTR    = 1 << 8,    // The "public" attribute, the field, struct or such is public to the importing files
                PROTECTED_ATTR = 1 << 9,    // The "protected" attribute, the field, struct or such is protected, meaning that it is visible only to inheriting classes
                PKG_PRIV_ATTR  = 1 << 10,   // The "package_private" attribute, the field, struct or such is visible only in to the members of the same namespace
                PRIVATE_ATTR   = 1 << 11,   // The "private" attribute, the field, struct or such is visible only to the members of the same file or underlying structure
            };

            friend inline constexpr Attributes operator|(Attributes a, Attributes b) {
                return static_cast<Attributes>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
            }

            friend inline constexpr bool operator&(Attributes a, Attributes b) {
                return (static_cast<uint16_t>(a) & static_cast<uint16_t>(b)) != 0;
            }

            friend inline constexpr Attributes& operator |=(Attributes& a, Attributes b) {
                a = a | b;
                return a;
            }

            // The symbol's type 
            SymbolType type;

            // The symbol's name
            std::string name;

            // The symbol's attributes, default to NONE
            Attributes attrs = Attributes::NO_ATTR;

            BaseSymbol(SymbolType t, std::string n, Attributes a) : type(t), name(std::move(n)), attrs(a) {}
            virtual ~BaseSymbol() = default;
        };

        /** A package stores names and other packages */
        struct PackageSymbol : public BaseSymbol {
            std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<BaseSymbol>>> symbols;

            PackageSymbol(std::string name, std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<BaseSymbol>>> symList, Attributes attrs) 
                : BaseSymbol(SymbolType::PACKAGE, std::move(name), attrs), symbols(std::move(symList)) {}
        };

        /** A type denotes what kind of data is stored inside a variable, constant, object, and so on, along the memory size occupied by this type */
        struct TypeSymbol : public BaseSymbol {
            enum class DataType  { INTEGER, FLOAT, CHAR, STRING, POINTER, REFERENCE };
            enum class Signature { UNSIGNED, SIGNED };

            DataType datType;
            Signature sig;
            unsigned int memorySize;

            TypeSymbol(std::string name, DataType dt, Signature s, unsigned int size, Attributes attrs) 
                : BaseSymbol(SymbolType::TYPE, std::move(name), attrs), datType(dt), sig(s), memorySize(size) {}
        };

        /** A function denotes a callable block of code, the function can contain parameters to pass data inside the function */
        struct FunctionSymbol : public BaseSymbol {
            struct Parameter { std::string name; TypeSymbol* type; };
            std::unique_ptr<iterableBuffer<Parameter>> parameters;
            TypeSymbol* returnType;

            FunctionSymbol(std::string name, std::unique_ptr<iterableBuffer<Parameter>> params, TypeSymbol* retType, Attributes attrs) :
                BaseSymbol(SymbolType::FUNCTION, std::move(name), attrs), returnType(retType) {}
        };

        /** An operator computes expressions and can add behaviour to the program, those are 100% customizable */
        struct OperatorSymbol : public BaseSymbol {
            struct Parameter { std::string name; TypeSymbol* type; };
            struct PrecedenceLevel { int left; int right; };

            std::unique_ptr<iterableBuffer<Parameter>> parameters;
            TypeSymbol* returnType;
            PrecedenceLevel priority;

            OperatorSymbol(std::string name, std::unique_ptr<iterableBuffer<Parameter>> params, TypeSymbol* retType, PrecedenceLevel p, Attributes attrs) :
                BaseSymbol(SymbolType::OPERATOR, std::move(name), attrs), returnType(retType), priority(std::move(p)) {}
        };

        /** A variable is a container whose data and values can be modified */
        struct VariableSymbol : public BaseSymbol {
            enum class ValueType { VALUE, REFERENCE, POINTER, INSTANCE };

            ValueType valueType;
            TypeSymbol* dataType;

            VariableSymbol(std::string name, TypeSymbol* dt, ValueType vt, Attributes attrs)
                : BaseSymbol(SymbolType::VARIABLE, std::move(name), attrs), dataType(dt), valueType(vt) {}
        };

        /** A constant is a container whose data and values cannot be modified */
        struct ConstantSymbol : public BaseSymbol {
            enum class ValueType { VALUE, REFERENCE, POINTER, INSTANCE };

            ValueType valueType;
            TypeSymbol* dataType;

            ConstantSymbol(std::string name, TypeSymbol* dt, ValueType vt, Attributes attrs)
                : BaseSymbol(SymbolType::VARIABLE, std::move(name), attrs), dataType(dt), valueType(vt) {}
        };

        /** A function set stores utility functions and extend the capability of other structs, or just exist as plain function sets, it's the C#'s equivalent to static classes */
        struct FunctionSetSymbol : public BaseSymbol {
            std::unique_ptr<iterableBuffer<FunctionSymbol>> functions;

            FunctionSetSymbol(std::string name, std::unique_ptr<iterableBuffer<FunctionSymbol>> fns, Attributes attrs)
                : BaseSymbol(SymbolType::FNSET, std::move(name), attrs), functions(std::move(fns)) {}
        };

        /** A record is a complex type that can contain only fields, the compiler might generate helper methods for each record made */
        struct RecordSymbol : public BaseSymbol {
            std::unique_ptr<iterableBuffer<BaseSymbol>> fields;

            RecordSymbol(std::string name, std::unique_ptr<iterableBuffer<BaseSymbol>> f, Attributes attrs) 
                : BaseSymbol(SymbolType::RECORD, std::move(name), attrs), fields(std::move(f)) {}
        };

        /** A structure is a complex type that can create complex objects, they can contain fields and methods */
        struct StructureSymbol : public BaseSymbol {
            std::unique_ptr<iterableBuffer<BaseSymbol>> fields;

            StructureSymbol(std::string name, std::unique_ptr<iterableBuffer<BaseSymbol>> f, Attributes attrs) 
                : BaseSymbol(SymbolType::STRUCTURE, std::move(name), attrs), fields(std::move(f)) {}
        };

        /** An interface is a strict abstract structure that can contain functions declarations to implement */
        struct InterfaceSymbol : public BaseSymbol {
            std::unique_ptr<iterableBuffer<FunctionSymbol>> funcs;

            InterfaceSymbol(std::string name, std::unique_ptr<iterableBuffer<FunctionSymbol>> f, Attributes attrs) 
                : BaseSymbol(SymbolType::INTERFACE, std::move(name), attrs), funcs(std::move(f)) {}
        };
    };
    
}

#endif