#include "translation/pLexer.hpp"


namespace photon {

#pragma region Private Methods

    bool pLexer::isWhitespace(const char c) {
        return std::isspace(static_cast<unsigned char>(c));
    }

    bool pLexer::isLetter(const char c) {
        return std::isalpha(c);
    }

    bool pLexer::isSymbol(const char c) {
        return (isItemPresentIn(c, symbols));
    }

    Token pLexer::readWord(const char BEGIN) {
        std::string lexeme;
        char c = BEGIN;
        lexeme += c;
            
        while (!isWhitespace(charBuffer.currentElement()) && !isSymbol(charBuffer.currentElement())) {  
            lexeme += charBuffer.advance();
        }

        // Checks if the lexeme refers to a known keyword
        if (isItemRefPresentIn(lexeme, reservedKeywords)) {
            return {TokenType::KEYWORD, lexeme};
        }

        return {TokenType::IDENTIFIER, lexeme};
    }

    Token pLexer::readSymbol(const char BEGIN) {
        std::string lexeme;
        char c = BEGIN;
        lexeme += c;

            
        while (isSymbol(charBuffer.currentElement())) {
            if (isItemPresentIn(charBuffer.currentElement(), combinableSymbols)) {
                lexeme += charBuffer.advance();
            } else {
                break;
            }
        }

        return {TokenType::SYMBOL, lexeme};
    }

    Token pLexer::readNumber() {
        size_t startPos = charBuffer.position() - 1;

        auto current_it = charBuffer.begin() + startPos;
        auto end_it = charBuffer.end();

        std::match_results<decltype(current_it)> match;

        for (const auto& rule : NUMBER_RULES) {
            if (std::regex_search(current_it, end_it, match, rule.second, std::regex_constants::match_continuous)) {
                std::string lexeme = match.str();

                size_t charsLeft = match.length() - 1;
                for (size_t i = 0; i < charsLeft; ++i) {
                    charBuffer.advance();
                }

                return { rule.first, lexeme };
            }
        }

        return { TokenType::INVALID, "invalid" };
    }

    Token pLexer::readStringOrChar(const char DELIMITER) {
        std::string lexeme = "";
            
        while (charBuffer.currentElement() != DELIMITER) {
            if (charBuffer.isAtEnd()) {
                return { TokenType::INVALID, "invalid" };
            }

            if (charBuffer.currentElement() == '\\') {
                charBuffer.advance();
                    
                if (charBuffer.isAtEnd()) {
                    return { TokenType::INVALID, "invalid" };
                }
                    
                char escaped = charBuffer.advance();

                switch (escaped) {
                    case 'n':  lexeme += '\n'; break; 
                    case 't':  lexeme += '\t'; break; 
                    case 'r':  lexeme += '\r'; break; 
                    case '"':  lexeme += '"';  break; 
                    case '\'': lexeme += '\''; break; 
                    case '\\': lexeme += '\\'; break; 
                    default: 
                        lexeme += '\\';
                        lexeme += escaped;
                        break;
                }
                continue;
            }

            lexeme += charBuffer.advance();
        }

        charBuffer.advance(); 

        if (DELIMITER == '\'') {
            return { TokenType::CHAR, lexeme };
        } else {
            return { TokenType::STRING, lexeme };
        }
    }

#pragma endregion Private Methods


#pragma region Public Methods

    explicit pLexer::pLexer(const PreprocessResult& PREP_RESULT) : ppfPath(PREP_RESULT.processedFilePath) {
        charBuffer.resetBuffer();
        tokenBuffer.resetBuffer();

        std::ifstream fileToProcess(ppfPath, std::ios::binary | std::ios::ate);
        if (!fileToProcess.is_open()) {
            auto logFrame = pLogger::buildFrame
            (
                IdPrefix::LEXER_LOG, 
                SeverityPrefix::ERR, 0x01, 
                {
                    "Cannot open file : \"", ppfPath, "\" as file does not exist at specified location\n", 
                    "Note that the char buffer will stay empty and will not be suitable for processing"
                }
            );
            pLogger::lprint(logFrame);
            return;
        }

        std::streamsize stsize = fileToProcess.tellg();
        fileToProcess.seekg(0, std::ios::beg);

        if (stsize > 0) {
            std::vector<char> buf(stsize);
            fileToProcess.read(buf.data(), stsize);
            charBuffer.setBuffer(std::move(buf));
        }
    }

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

#pragma endregion Public Methods

} // namespace photon