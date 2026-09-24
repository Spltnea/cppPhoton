#ifndef P_PARSER_HPP
#   define P_PARSER_HPP

#include "intermediateObjects/pToken.hpp"
#include "intermediateObjects/pNode.hpp"
#include "intermediateObjects/pSymbol.hpp"

#include "utils/pLogger.hpp"
#include "utils/streams/ostream_extensions.hpp"
#include "utils/types/iterableBuffer.hpp"
#include "utils/vectorUtilities.hpp"

#include "helpers/reservedEntries.hpp"

#include <unordered_map>
#include <string>

namespace photon {
    
    /** Transforms a token array into an Abstract Syntax Tree (AST) */
    class pParser {
    private :
        iterableBuffer<Node> ast;       // The AST container
        
        /** Advances if the current token lexeme matches the given value */
        void expectAndAdvance(iterableBuffer<Token> tokBuf, std::string value);

        /** Parses an expression left hand side */
        std::unique_ptr<ExpressionNode> parsePrefix(Token& tok);

        /** Parses an expression according to a minimal precedence level */
        std::unique_ptr<ExpressionNode> parseExpression(iterableBuffer<Token> tokBuf, int minPrecLevel);

        /** Applies the first pass, which gets symbols and stores them into a preliminary symbol table */
        void applyFirstPass(iterableBuffer<Token> tokBuf);

    public:
        /** Transforms the token stream into an AST */
        void applyParserPass(iterableBuffer<Token> tokBuf);
    };
}


#endif