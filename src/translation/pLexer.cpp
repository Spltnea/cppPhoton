#include "translation/pLexer.hpp"


namespace photon {
    void pLexer::applyLexerPass() {
        if (charBuffer.isEmpty()) {
            auto logFrame = pLogger::buildFrame
            (
                IdPrefix::LEXER_LOG, SeverityPrefix::ERR, 0x02, 
                {"Empty char buffer, will not resume lexing process" }
            );

            pLogger::lprint(logFrame);
            tokenBuffer.addElement({lineNo, colNo, TokenType::END_FILE, "EOF"}); 

            return;
        } 
            
        // Main lexing loop
        while (!charBuffer.isAtEnd()) {
            char c = charBuffer.advance();

            // Check for new lines and update the positions accordingly
            if (isNewLine(c)) { lineNo++; colNo++; }

            // Check for identifiers and keywords
            if (isLetter(c) || c == '_') { tokenBuffer.addElement(readWord(c)); } 
        }

        tokenBuffer.addElement({lineNo, colNo, TokenType::END_FILE, "EOF"}); 
    }

} // namespace photon
