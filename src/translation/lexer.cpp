// AI Helps for REGEX related methods

#include "lexer.hpp"

namespace photon {
#pragma region Private Methods

    void Lexer::flushBuffers() {
        sourceBuffer.clear();
        internalBuffer.clear();
    }

    // == Processing Utilities ==

    [[nodiscard]] bool Lexer::isLetter(const char c) {
        static const std::regex matchingRule(R"([a-zA-Z])");
        return std::regex_match(&c, &c + 1, matchingRule);
    }

    [[nodiscard]] bool Lexer::isDigit(const char c) {
        static const std::regex matchingRule(R"([0-9])");
        return std::regex_match(&c, &c + 1, matchingRule);
    }

    [[nodiscard]] bool Lexer::isSimpleSymbol(const char c) {
        return ReservedEntries::isEntryIn(c, ReservedEntries::soloSymbols);
    }

    [[nodiscard]] bool Lexer::isCombinableSymbol(const char c) {
        return ReservedEntries::isEntryIn(c, ReservedEntries::combinableSymbols);
    }

    [[nodiscard]] bool Lexer::isGenericSymbol(const char c) {
        return (
            ReservedEntries::isEntryIn(c, ReservedEntries::combinableSymbols) ||
            ReservedEntries::isEntryIn(c, ReservedEntries::soloSymbols)
        );
    }

    [[nodiscard]] Lexer::Token Lexer::createWordToken() {
        static const std::regex matchingRule(R"([a-zA-Z0-9_]+)");
        
        const char* current = sourceBuffer.currentElementPointer();
        const char* end     = sourceBuffer.endOfBufferPointer();
        std::cmatch match;

        if (std::regex_search(current, end, match, matchingRule, std::regex_constants::match_continuous)) {
            std::string lexeme = match[0].str();
            sourceBuffer.advance(match.length(0));

            return (ReservedEntries::isEntryRefIn(lexeme, ReservedEntries::keywordTable) 
                    ? Token{Token::TokenType::_KEYWORD, std::move(lexeme), sourceBuffer.cursorPosition()}
                    : Token{Token::TokenType::_IDENTIFIER, std::move(lexeme), sourceBuffer.cursorPosition()});
        }

        sourceBuffer.advance();
        return {Token::TokenType::_INVALID, "INVALID", sourceBuffer.cursorPosition()};
    }

    [[nodiscard]] Lexer::Token Lexer::createNumberToken() {
        static const std::regex matchingRule(
            R"(0[xX][0-9a-fA-F](_?[0-9a-fA-F])*|)"
            R"(0[bB][01](_?[01])*|)"
            R"(0[oO][0-7](_?[0-7])*|)"
            R"([0-9](_?[0-9])*)"
            R"((\.[0-9](_?[0-9])*)?)"
            R"(([eE][+-]?[0-9](_?[0-9])*)?)"
        );

        const char* current = sourceBuffer.currentElementPointer();
        const char* end     = sourceBuffer.endOfBufferPointer();
        std::cmatch match;

        if (std::regex_search(current, end, match, matchingRule, std::regex_constants::match_continuous)) {
            std::string lexeme = match[0].str();
            sourceBuffer.advance(match.length(0));

            return {Token::TokenType::_NUMBER, std::move(lexeme), sourceBuffer.cursorPosition()};
        }

        sourceBuffer.advance();
        return {Token::TokenType::_INVALID, "INVALID", sourceBuffer.cursorPosition()};
    }

    [[nodiscard]] Lexer::Token Lexer::createSymbolToken() {
        char c = sourceBuffer.currentElement();

        if (isSimpleSymbol(c)) { 
            sourceBuffer.advance();
            return { Token::TokenType::_SYMBOL, std::string(1, c), sourceBuffer.cursorPosition() };
        }
        
        static const std::regex matchingRule(R"([+\-*/%=<>&|~!\^?.:]+)");
        
        const char* current = sourceBuffer.currentElementPointer();
        const char* end     = sourceBuffer.endOfBufferPointer();
        std::cmatch match;

        if (std::regex_search(current, end, match, matchingRule, std::regex_constants::match_continuous)) {
            std::string lexeme = match[0].str();

            sourceBuffer.advance(match.length(0));

            return {Token::TokenType::_SYMBOL, std::move(lexeme), sourceBuffer.cursorPosition()};
        }

        sourceBuffer.advance();
        return {Token::TokenType::_INVALID, "INVALID", sourceBuffer.cursorPosition()};
    }

    [[nodiscard]] Lexer::Token Lexer::createTextualConstantToken() {

        char c = sourceBuffer.currentElement();

        if (c == '\'') {
            static const std::regex matchingRule(R"('(?:[^'\\\r\n]|\\.)*')");

            const char* current = sourceBuffer.currentElementPointer();
            const char* end     = sourceBuffer.endOfBufferPointer();
            std::cmatch match;

            if (std::regex_search(current, end, match, matchingRule, std::regex_constants::match_continuous)) {
                std::string lexeme = match[0].str();

                sourceBuffer.advance(match.length(0));

                return { Token::TokenType::_CHAR, std::move(lexeme), sourceBuffer.cursorPosition() };
            }

            sourceBuffer.advance();
            return { Token::TokenType::_INVALID, "'", sourceBuffer.cursorPosition() };
        }

        static const std::regex matchingRule(R"("(?:[^"\\\r\n]|\\.)*")");

        const char* current = sourceBuffer.currentElementPointer();
        const char* end     = sourceBuffer.endOfBufferPointer();
        std::cmatch match;

        if (std::regex_search(current, end, match, matchingRule, std::regex_constants::match_continuous)) {
            std::string symbol = match[0].str();

            sourceBuffer.advance(match.length(0));

            return {Token::TokenType::_STRING, std::move(symbol), sourceBuffer.cursorPosition()};
        }

        sourceBuffer.advance();
        return { Token::TokenType::_INVALID, "\"", sourceBuffer.cursorPosition() };
    }

#pragma endregion

#pragma region Public Methods

    Lexer::LexerOutput Lexer::buildOutput() {
        Lexer::LexerOutput output = { std::move(internalBuffer) };
        flushBuffers();

        return output;
    }

    void Lexer::process(Preprocessor::PreprocessorOutput inputPayload) {
        sourceBuffer = std::move(inputPayload.outputBuffer);
        char c;

        while (!sourceBuffer.isAtEnd()) {
            c = sourceBuffer.currentElement();

            // Newlines characters (\n) skips
            if (std::isspace(static_cast<unsigned char>(c))) { 
                sourceBuffer.advance(); 
                continue; 
            }

            // Textual constants reading
            if (c == '\'' || c == '"') { internalBuffer.addElement(createTextualConstantToken()); continue; }

            // Numbers reading
            if (isDigit(c)) { internalBuffer.addElement(createNumberToken()); continue; }

            // Identifiers and Keywords reading
            if (isLetter(c) || c == '_') { internalBuffer.addElement(createWordToken()); continue; }

            // Symbols reading
            if (isGenericSymbol(c)) { internalBuffer.addElement(createSymbolToken()); continue; }

            sourceBuffer.advance();
            internalBuffer.addElement({ Token::TokenType::_INVALID, "INVALID", sourceBuffer.cursorPosition()});
        }

        internalBuffer.addElement({Token::TokenType::_EOF, "EOF", sourceBuffer.cursorPosition()});
    }

#pragma endregion
}