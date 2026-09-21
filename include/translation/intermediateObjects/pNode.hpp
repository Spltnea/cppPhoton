#ifndef P_NODE_HPP
#   define P_NODE_HPP

#include <memory>
#include <utility>

#include "utils/types/iterableBuffer.hpp"
#include "pToken.hpp"


namespace photon {

#pragma region Base Classes

    // Base class for all nodes
    class Node {
    public:
        virtual ~Node() = default;
    };

    // Base class for expression related nodes
    class ExpressionNode : public Node {
    public:
        ~ExpressionNode() override = default;
    };

    // Base class for statement related nodes
    class StatementNode : public Node {
    public:
        ~StatementNode() override = default;
    };

    // Base class for entry sets nodes like modifiers, parameters, and such
    class EntrySetNode : public Node {
    public:
        ~EntrySetNode() override = default;
    };

#pragma endregion Base Classes

#pragma region Expression Nodes

    /** A node whose value is a constant literal */
    class ConstantExprNode : public ExpressionNode {
    private:
        Token value;
    public:
        explicit ConstantExprNode(Token v) : value(std::move(v)) {}
    };

    /** A node that holds two other expressions with an operator token */
    class BinaryExpressionNode : public ExpressionNode {
    private:
        std::unique_ptr<ExpressionNode> leftOperand;
        std::unique_ptr<ExpressionNode> rightOperand;
        Token operatorToken;

    public:
        explicit BinaryExpressionNode(std::unique_ptr<ExpressionNode> lhs, std::unique_ptr<ExpressionNode> rhs, Token opTok) 
            : leftOperand(std::move(lhs)), rightOperand(std::move(rhs)), operatorToken(std::move(opTok)) {}        
    };

    /** A node that holds one expression with an operator, the expression is prefixed by the operator */
    class PrefixedUnaryExpressionNode : public ExpressionNode {
    private:
        std::unique_ptr<ExpressionNode> operand;
        Token operatorToken;
    public:
        explicit PrefixedUnaryExpressionNode(std::unique_ptr<ExpressionNode> oprd, Token opTok) 
            : operand(std::move(oprd)), operatorToken(std::move(opTok)) {}
    };

    /** A node that holds one expression with an operator, the expression is postfixed by the operator */
    class PostfixedUnaryExpressionNode : public ExpressionNode {
    private:
        std::unique_ptr<ExpressionNode> operand;
        Token operatorToken;
    public:
        explicit PostfixedUnaryExpressionNode(std::unique_ptr<ExpressionNode> oprd, Token opTok) 
            : operand(std::move(oprd)), operatorToken(std::move(opTok)) {}
    };

    /** A node that manages function calls */
    class FunctionCallExpressionNode : public ExpressionNode {
    private:
        std::string functionName;
        std::unique_ptr<iterableBuffer<ExpressionNode>> passedArguments;
    public:
        explicit FunctionCallExpressionNode(std::string name, std::unique_ptr<iterableBuffer<ExpressionNode>> args) : functionName(std::move(name)), passedArguments(std::move(args)) {}
    };

    /** A node wrapped in parenthesis */
    class ParenthesisExpressionNode : public ExpressionNode {
    private:
        std::unique_ptr<ExpressionNode> operand;
    public:
        explicit ParenthesisExpressionNode(std::unique_ptr<ExpressionNode> op) : operand(std::move(op)) {}
    };

    /** A node that holds one expression as a statement */
    class StatementExpressionNode : public ExpressionNode {
    private:
        std::unique_ptr<StatementNode> stmt;
    public:
        explicit StatementExpressionNode(std::unique_ptr<StatementNode> statement) : stmt(std::move(statement)) {}
    };

#pragma endregion Expression Nodes

#pragma region Structural Nodes

/** A node managing parameter declarations in functions */
class ParameterDeclarationNode : public EntrySetNode {
private:
    std::string paramName;
    std::string paramType;
    std::unique_ptr<ExpressionNode> defaultValue;

public:
    explicit ParameterDeclarationNode(std::string name, std::string type, std::unique_ptr<ExpressionNode> value) : paramName(std::move(name)), paramType(std::move(type)), defaultValue(std::move(value)) {}
};

#pragma enderegion Structural Nodes

#pragma region Statement Nodes

    // == Variables Related Statements ==

    /** A node managing variables declarations with optional initializations */
    class VariableDeclarationNode : public StatementNode {
    private:
        std::string variableName;
        std::string variableType;
        std::unique_ptr<ExpressionNode> variableValue;
    public:
        explicit VariableDeclarationNode(std::string varName, std::string varType, std::unique_ptr<ExpressionNode> varValue)
            : variableName(std::move(varName)), variableType(std::move(varType)), variableValue(std::move(varValue)) {}
    };

    /** A node managing values assignements to variables */
    class VariableAssignmentNode : public StatementNode {
    private:
        std::string targetName;
        std::unique_ptr<ExpressionNode> assignmentValue;
    public:
        explicit VariableAssignmentNode(std::string varName, std::unique_ptr<ExpressionNode> assgnmValue)
            : targetName(std::move(varName)), assignmentValue(std::move(assgnmValue)) {}
    };

    /** A node managing constants definitions */
    class ConstantDefinitionNode : public StatementNode {
    private:
        std::string constantName;
        std::string constantType;
        std::unique_ptr<ExpressionNode> constantValue;
    public:
        explicit ConstantDefinitionNode(std::string constName, std::string constType, std::unique_ptr<ExpressionNode> constValue)
            : constantName(std::move(constName)), constantType(std::move(constType)), constantValue(std::move(constValue)) {}
    };

    /** A node managing pointers definitions */
    class PointerDeclarationNode : public StatementNode {
    private:
        std::string pointerName;
        std::string pointerType;
        std::unique_ptr<ExpressionNode> pointerAddress;
    public:
        explicit PointerDeclarationNode(std::string name, std::string type, std::unique_ptr<ExpressionNode> address) : pointerName(std::move(name)), pointerType(std::move(type)), pointerAddress(std::move(address)) {}
    };

    /** A node managing adresses assignements to pointers */
    class PointerAssignmentNode : public StatementNode {
    private:
        std::string targetName;
        std::unique_ptr<ExpressionNode> targetAddress;
    public:
        explicit PointerAssignmentNode(std::string name, std::unique_ptr<ExpressionNode> address) : targetName(std::move(name)), targetAddress(std::move(address)) {}
    };

    /** A node managing constant pointers definitions */
    class ConstantPointerDefinitionNode : public StatementNode {
    private:
        std::string pointerName;
        std::string pointerType;
        std::unique_ptr<ExpressionNode> pointerValue;
    public:
        explicit ConstantPointerDefinitionNode(std::string name, std::string type, std::unique_ptr<ExpressionNode> value) : pointerName(std::move(name)), pointerType(std::move(type)), pointerValue(std::move(value)) {}
    };

    // == Definitions ==

    /** A node managing function definitions */
    class FunctionDefinitionNode : public StatementNode {
    private:
        std::string functionName;
        std::string functionReturnType;
        std::unique_ptr<iterableBuffer<ParameterDeclarationNode>> functionParameters;
    public:
        explicit FunctionDefinitionNode(std::string name, std::string returnType, std::unique_ptr<iterableBuffer<ParameterDeclarationNode>> params)
            : functionName(std::move(name)), functionReturnType(std::move(returnType)), functionParameters(std::move(params)) {}
    };

    // == Block Based Statements ==

    /** A node whose contents are a block of other statements nodes */
    class BlockStatementNode : public StatementNode {
    private:
        std::unique_ptr<iterableBuffer<StatementNode>> statements;
    public:
        explicit BlockStatementNode(std::unique_ptr<iterableBuffer<StatementNode>> stmts) : statements(std::move(stmts)) {}
    };

    /** A node managing if statements */
    class IfStatementNode : public StatementNode {
    private:
        std::unique_ptr<ExpressionNode> condition;
        std::unique_ptr<BlockStatementNode> statements;
        std::unique_ptr<BlockStatementNode> elseBranch;
    public:
        // Creates a if stmt node without an "else" branch
        explicit IfStatementNode(std::unique_ptr<ExpressionNode> cond, std::unique_ptr<BlockStatementNode> stmts) 
            : condition(std::move(cond)), statements(std::move(stmts)) {}

        // Creates a if stmt node with an "else" branch
        explicit IfStatementNode(std::unique_ptr<ExpressionNode> cond, std::unique_ptr<BlockStatementNode> stmts, std::unique_ptr<BlockStatementNode> elseBrnch)
            : condition(std::move(cond)), statements(std::move(stmts)), elseBranch(std::move(elseBrnch)) {}
    };

    /** A node managing while statements */
    class WhileStatementNode : public StatementNode {
    private:
        std::unique_ptr<ExpressionNode> condition;
        std::unique_ptr<BlockStatementNode> statements;
    public:
        explicit WhileStatementNode(std::unique_ptr<ExpressionNode> cond, std::unique_ptr<BlockStatementNode> stmts) 
            : condition(std::move(cond)), statements(std::move(stmts)) {}
    };

    // == Flow Control ==

    /** A node managing return statements */
    class ReturnStatementNode : public StatementNode {
    private:
        std::string returningFunction;
        std::string returnValue;
    public:
        explicit ReturnStatementNode(std::string retFunc, std::string retVal) : returningFunction(std::move(retFunc)), returnValue(std::move(retVal)) {}
    };

#pragma endregion Statement Nodes

} // namespace photon

#endif // P_NODE_HPP