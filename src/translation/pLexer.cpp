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
            tokenBuffer.addElement({TokenType::END_FILE, "EOF"}); 

            return;
        } 
            
        // Main lexing loop
        while (!charBuffer.isAtEnd()) {
            char c = charBuffer.advance();

            // Check for whitespaces to ignore
            if (isWhitespace(c)) { continue; }

            // Check for identifiers and keywords
            if (isLetter(c) || c == '_') { 
                tokenBuffer.addElement(readWord(c)); 
                continue;
            }

            // Check for numbers
            if (std::isdigit(c) || (c == '.' && std::isdigit(charBuffer.currentElement()))) {
                tokenBuffer.addElement(readNumber());
                continue;
            }

            // Check for strings and chars
            if (c == '\'' || c == '"') {
                tokenBuffer.addElement(readStringOrChar(c));
                continue;
            }

            // Check for symbols
            if (isSymbol(c)) { 
                tokenBuffer.addElement(readSymbol(c)); 
                continue;
            } 

            // Fallback
            else {
                tokenBuffer.addElement({ TokenType::INVALID, "invalid" });
            }
        }

        tokenBuffer.addElement({TokenType::END_FILE, "EOF"}); 
    }

} // namespace photon
