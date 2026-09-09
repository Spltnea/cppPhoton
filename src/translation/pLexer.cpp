#include "translation/pLexer.hpp"

namespace photon {
    void pLexer::applyLexerPass() {
        
        // Main lexing loop
        while (!isAtEnd()) {
        }

        tokens.push_back({lineNo, colNo, TokenType::END_FILE, "EOF"}); 
    }
} // namespace photon
