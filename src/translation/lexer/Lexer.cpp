// AI Helps for REGEX related methods

module Lexer;

import Preprocessor;
import std;

namespace photon {
#pragma region Private Methods

    void Lexer::flushBuffers() noexcept {
        sourceBuffer.clear();
        internalBuffer.clear();
    }

    // == Processing Utilities ==

    [[nodiscard]] constexpr bool Lexer::isLetter(const char c) noexcept {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
    }

    [[nodiscard]] constexpr bool Lexer::isDigit(const char c) noexcept {
        return c >= '0' && c <= '9';
    }

    [[nodiscard]] constexpr bool Lexer::isSimpleSymbol(const char c) noexcept {
        return ReservedEntries::isEntryIn(c, ReservedEntries::soloSymbols);
    }

    [[nodiscard]] constexpr bool Lexer::isCombinableSymbol(const char c) noexcept {
        return ReservedEntries::isEntryIn(c, ReservedEntries::combinableSymbols);
    }

    [[nodiscard]] constexpr bool Lexer::isGenericSymbol(const char c) noexcept {
        return (
            ReservedEntries::isEntryIn(c, ReservedEntries::combinableSymbols) ||
            ReservedEntries::isEntryIn(c, ReservedEntries::soloSymbols)
        );
    }

    [[nodiscard]] Lexer::Token Lexer::createWordToken() noexcept {
        static const std::regex matchingRule(R"([a-zA-Z0-9_]+)");
        
        const char* current = sourceBuffer.currentElementPointer();
        const char* end     = sourceBuffer.endOfBufferPointer();
        std::cmatch match;

        if (std::regex_search(current, end, match, matchingRule, std::regex_constants::match_continuous)) {
            std::string_view lexeme(&*match[0].first, match.length(0));
            sourceBuffer.advance(match.length(0));

            return (ReservedEntries::isEntryRefIn(lexeme, ReservedEntries::keywordTable) 
                    ? Token{Token::TokenType::_KEYWORD, lexeme, sourceBuffer.cursorPosition()}
                    : Token{Token::TokenType::_IDENTIFIER, lexeme, sourceBuffer.cursorPosition()});
        }

        sourceBuffer.advance();
        return {Token::TokenType::_INVALID, "INVALID", sourceBuffer.cursorPosition()};
    }

    [[nodiscard]] Lexer::Token Lexer::createNumberToken() noexcept {
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
            std::string_view lexeme(&*match[0].first, match.length(0));
            sourceBuffer.advance(match.length(0));

            return {Token::TokenType::_NUMBER, std::move(lexeme), sourceBuffer.cursorPosition()};
        }

        sourceBuffer.advance();
        return {Token::TokenType::_INVALID, "INVALID", sourceBuffer.cursorPosition()};
    }

    [[nodiscard]] Lexer::Token Lexer::createSymbolToken() noexcept {
        char c = sourceBuffer.currentElement();

        if (isSimpleSymbol(c)) { 
            sourceBuffer.advance();
            return { Token::TokenType::_SYMBOL, std::string_view(sourceBuffer.currentElementPointer(), 1), sourceBuffer.cursorPosition() };
        }
        
        static const std::regex matchingRule(R"([+\-*/%=<>&|~!\^?.:]+)");
        
        const char* current = sourceBuffer.currentElementPointer();
        const char* end     = sourceBuffer.endOfBufferPointer();
        std::cmatch match;

        if (std::regex_search(current, end, match, matchingRule, std::regex_constants::match_continuous)) {
            std::string_view lexeme(&*match[0].first, match.length(0));

            sourceBuffer.advance(match.length(0));

            return {Token::TokenType::_SYMBOL, std::move(lexeme), sourceBuffer.cursorPosition()};
        }

        sourceBuffer.advance();
        return {Token::TokenType::_INVALID, "INVALID", sourceBuffer.cursorPosition()};
    }

    [[nodiscard]] Lexer::Token Lexer::createTextualConstantToken() noexcept {

        char c = sourceBuffer.currentElement();

        if (c == '\'') {
            static const std::regex matchingRule(R"('(?:[^'\\\r\n]|\\.)*')");

            const char* current = sourceBuffer.currentElementPointer();
            const char* end     = sourceBuffer.endOfBufferPointer();
            std::cmatch match;

            if (std::regex_search(current, end, match, matchingRule, std::regex_constants::match_continuous)) {
                std::string_view lexeme(&*match[0].first, match.length(0));

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
            std::string_view lexeme(&*match[0].first, match.length(0));

            sourceBuffer.advance(match.length(0));

            return {Token::TokenType::_STRING, std::move(lexeme), sourceBuffer.cursorPosition()};
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