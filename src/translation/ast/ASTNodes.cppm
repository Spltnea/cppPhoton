module;

export module ASTNodes;

import BaseASTNodes;
import std;

namespace photon {
    
#pragma region Expression Nodes

    /// @brief A node whose value is a constant
    struct ConstantExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_CONSTANT_EXPR;
        std::string_view value;

        ConstantExpressionNode(std::string_view val, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), value(val) {}
    };

    /// @brief A node whose value is an identifier
    struct IdentifierExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_IDENTIFIER_EXPR;
        std::string_view identifier;

        IdentifierExpressionNode(std::string_view idf, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), identifier(idf) {}
    };

    /// @brief A node whose value is an expression prefixed by an operator
    struct PrefixedExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_PREFIX_UNARY_EXPR;
        std::string_view operatorSymbol;
        GenericNode* operand;

        PrefixedExpressionNode(std::string_view osym, GenericNode* opd, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), operatorSymbol(osym), operand(opd) {}
    };

    /// @brief A node whose value is an expression postfixed by an operator
    struct PostfixedExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_POSTFIX_UNARY_EXPR;
        std::string_view operatorSymbol;

        GenericNode* operand;

        PostfixedExpressionNode(std::string_view osym, GenericNode* opd, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), operatorSymbol(osym), operand(opd) {}
    };

    /// @brief A node whose value is two expressions between one operator
    struct BinaryExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_BINARY_EXPR;
        std::string_view operatorSymbol;

        GenericNode* leftOperand;
        GenericNode* rightOperand;

        BinaryExpressionNode(std::string_view osym, GenericNode* lhs, GenericNode* rhs, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), leftOperand(lhs), rightOperand(rhs) {}
    };

    /// @brief A node whose value is a member access
    struct MemberAccessExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_MEMBER_ACCESS_EXPR;

        std::string_view identifier;
        std::string_view member;

        MemberAccessExpressionNode(std::string_view idt, std::string_view mem, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), identifier(idt), member(mem) {}
    };

    /// @brief A node whose value is an array access
    struct ArrayAccessExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_ARRAY_ACCESS_EXPR;

        std::string_view identifier;
        GenericNode* index;

        ArrayAccessExpressionNode(std::string_view idt, GenericNode* idx, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), identifier(idt), index(idx) {}
    };

    /// @brief A node whose value is a function call
    struct FunctionCallExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_FUNC_CALL_EXPR;

        std::string_view identifier;
        std::span<GenericNode*> arguments;

        FunctionCallExpressionNode(std::string_view idt, std::span<GenericNode*> args, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), identifier(idt), arguments(args) {}
    };

    /// @brief A node whose value is a statement wrapped inside an expression
    struct StatementExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_STATEMENT_EXPR;

        StatementNode* statement;

        StatementExpressionNode(StatementNode* stmt, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), statement(stmt) {}
    };

    /// @brief A node whose value is an expression wrapped in parenthesis
    struct ParenthesizedExpressionNode : public ExpressionNode {
        static constexpr NodeType type = NodeType::_PARENTHESIZED_EXPR;

        ExpressionNode* expression;

        ParenthesizedExpressionNode(ExpressionNode* expr, SourceLocation loc = {}) noexcept : ExpressionNode(type, loc), expression(expr) {}
    };

#pragma endregion Expression Nodes

#pragma region Statement Nodes

    

#pragma endregion Statement Nodes

}