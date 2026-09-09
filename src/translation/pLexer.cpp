#include "translation/pLexer.hpp"
#include <iostream>

using std::cout;

namespace photon {
    void pLexer::applyLexerPass() {
        if (buffer.empty()) {
            auto logFrame = pLogger::buildFrame(IdPrefix::LEXER_LOG, 
                                                        SeverityPrefix::ERR, 0x02, 
                                                        {"Empty char buffer, will not resume lexing process"
                                                        });
            pLogger::lprint(logFrame);
            tokens.push_back({lineNo, colNo, TokenType::END_FILE, "EOF"}); 

            return;
        } 
            
        // Main lexing loop
        while (!isAtEnd()) {
            char c = returnAndAdvance();

            // Check for new lines and update the positions accordingly
            if (isNewLine(c)) { lineNo++; colNo++; }
        }

        tokens.push_back({lineNo, colNo, TokenType::END_FILE, "EOF"}); 
    }

} // namespace photon
