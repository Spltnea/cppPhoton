#include "translation/pParser.hpp"

namespace photon {

#pragma region Private Methods

    void pParser::expectAndAdvance(iterableBuffer<Token> tokBuf, std::string value) {
        if (tokBuf.currentElement().lexeme == value) { tokBuf.advance(); }
        else { throw std::runtime_error("The current token does not match the expected one"); }
    }

    std::unique_ptr<ExpressionNode> pParser::parsePrefix(Token& tok) {
        // Constant or identifier
        if (isNum(tok) || tok.type == TokenType::IDENTIFIER) {
            return std::make_unique<ConstantExprNode>(tok);
        }

        // Null values
        if (tok.lexeme == "nullptr" || tok.lexeme == "nullval") {
            return std::make_unique<ConstantExprNode>(tok);
        }
        
        // Expression wrapped in parenthesis
        if (tok.lexeme == "(") {
            auto expr = parseExpression(0);
            expectAndAdvance(")");
            return std::make_unique<ParenthesisExpressionNode>(std::move(expr));
        }

        // Prefixed unary expression
        if (!tok.lexeme.empty() && isItemPresentIn(tok.lexeme[0], combinableSymbols)) {
            constexpr int PREC_UNARY = 130; // Highest Priority
            auto operand = parseExpression(PREC_UNARY);
            return std::make_unique<PrefixedUnaryExpressionNode>(std::move(operand), tok);
        }

        // Malformed expression
        throw std::runtime_error("Malformed Expression around " + tok.lexeme);
    }

    std::unique_ptr<ExpressionNode> pParser::parseExpression(iterableBuffer<Token> tokBuf, int minPrecLevel) {
        Token t = tokBuf.advance();
        std::unique_ptr<ExpressionNode> lhs = parsePrefix(t); 

        while (true) {
            const auto& next = tokBuf.currentElement();

            if (next.lexeme.empty() || !isItemPresentIn(next.lexeme[0], combinableSymbols)) { break; }
            PrecedenceLevel prec = getInfixPrecedenceOf(next.lexeme[0]);

            // Parent takes over if the current op priority is lower to the threshold
            if (prec.left < minPrecLevel) { break; } 
            
            Token opTok = tokBuf.advance();
            auto rhs = parseExpression(prec.right);

            lhs = std::make_unique<BinaryExpressionNode>(
                std::move(lhs),
                std::move(rhs),
                std::move(opTok)
            );
        }

        return lhs;
    }

    void pParser::applyFirstPass(iterableBuffer<Token> tokBuf) {}

#pragma endregion Private Methods

#pragma region Public Methods

    void pParser::applyParserPass(iterableBuffer<Token> tokBuf) {

        while (!tokBuf.isAtEnd()) {
            Token t = tokBuf.advance();
        }
    }

#pragma endregion Public Methods
}