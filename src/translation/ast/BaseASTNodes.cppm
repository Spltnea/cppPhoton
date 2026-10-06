module;

export module BaseASTNodes;

import std;

export namespace photon {

    /// @brief Contains metadatas to deduce token locations
    struct SourceLocation { 
        std::uint32_t offset{0}; 
        std::uint32_t length{0};

        constexpr SourceLocation() noexcept : offset(0), length(0) {}
    };

    /// @brief A generic node representing usefull structures of code
    struct GenericNode {
        /// @brief Represents every type of node
        enum class NodeType : std::uint8_t {
            // Expressions
            _EXPR_START,
            _CONSTANT_EXPR = _EXPR_START,       // An expression composed of a litteral
            _IDENTIFIER_EXPR,                   // An expression composed of an identifier
            _PREFIX_UNARY_EXPR,                 // An expression composed of an operator and an operand
            _POSTFIX_UNARY_EXPR,                // An expression composed of an operand and an operator
            _BINARY_EXPR,                       // An expression composed of two operands and an operator
            _MEMBER_ACCESS_EXPR,                // A binary expression composed of two identifiers (the object, and the member to access)
            _ARRAY_ACCESS_EXPR,                 // A postfixed unary expression composed of an identifier and an operand (array index)
            _FUNC_CALL_EXPR,                    // An expression composed of a function call
            _STATEMENT_EXPR,                    // A statement wrapped in an expression
            _PARENTHESIZED_EXPR,                // An expression wrapped in parenthesis
            _EXPR_END = _PARENTHESIZED_EXPR,
            
            // Statements
            _STMT_START,
            _VARDECL_STMT = _STMT_START,        // A variable declaration statement
            _VARDEF_STMT,                       // A variable definition statement
            _CONSTDEF_STMT,                     // A constant definition statement

            _FNDECL_STMT,                       // A function declaration statement
            _FNDEF_STMT,                        // A function definition statement

            _OPERATORDECL_STMT,                 // An operator declaration statement
            _OPERATORDEF_STMT,                  // An operator definition statement

            _PACKAGEDEF_STMT,                   // A package definition statement

            _STRUCTDEF_STMT,                    // A structure definition statement
            _ENUMDEF_STMT,                      // An enumeration definition statement
            _RECORDDEF_STMT,                    // A record definition statement
            _FNSETDEF_STMT,                     // A function set definition statement

            _TYPEDEF_STMT,                      // An user type definition statement

            _IF_STMT,                           // An "if" statement
            _WHILE_STMT,                        // A "while" statement
            _SWITCH_STMT,                       // A "switch" statement

            _RETURN_STMT,                       // A "return" statement
            _BREAK_STMT,                        // A "break" statement
            _CONTINUE_STMT,                     // A "continue" statement
            _THROW_STMT,                        // A "throw" statement
            
            _TRY_STMT,                          // A "try" statement
            _INTERCEPT_STMT,                    // An "intercept" statement

            _EXPRESSION_STMT,                   // An expression treated as a statement
            _STMT_END = _EXPRESSION_STMT,
        };

        /// @brief The node's type
        NodeType nodeType;

        /// @brief The target tokens location
        SourceLocation location;

        constexpr GenericNode(NodeType t, SourceLocation loc = {}) noexcept : nodeType(t), location(loc) {}

        /// @brief Asserts that the member node is of the given template's type
        /// @tparam T The type to match against
        template<typename T>
        constexpr bool is() const noexcept { 
            if constexpr (requires { T::classof(this); }) {
                return T::classof(this);
            } else {
                return nodeType == T::type;
            }
        }

        /// @brief Asserts that the member node is of the given template's type
        /// @tparam T The type to match against
        template<typename T>
        constexpr bool is(const GenericNode* node) noexcept { return node != nullptr && node->is<T>();  }
        
        /// @brief Casts a node to an another type, returns nullptr on failure
        /// @tparam T The type to cast the node at 
        template<typename T>
        constexpr T* as(GenericNode* node) noexcept { return node ? node->as<T>() : nullptr; }
        
        /// @brief Casts a node to an another type, returns nullptr on failure
        /// @tparam T The type to cast the node at 
        template<typename T>
        constexpr const T* as(const GenericNode* node) noexcept { return node ? node->as<T>() : nullptr; }
    };

    /// @brief A generic expression node
    struct ExpressionNode : public GenericNode {
        using GenericNode::GenericNode;
        static constexpr bool classof(const GenericNode* n) noexcept { return n->nodeType >= NodeType::_EXPR_START && n->nodeType <= NodeType::_EXPR_END; }
    };

    /// @brief A generic statement node
    struct StatementNode : public GenericNode {
        using GenericNode::GenericNode;

        static constexpr bool classof(const GenericNode* n) noexcept { 
            return n->nodeType >= NodeType::_STMT_START && n->nodeType <= NodeType::_STMT_END; 
        }
    };
}