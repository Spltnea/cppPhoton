export module Lexer;

import std;
import Preprocessor;
import IterableBuffer;

namespace photon {

    /// @brief Has multiple collections of arrays and views containing reserved entries
    struct ReservedEntries {

        /// @brief A table containing keywords
        static constexpr std::array<std::string_view, 36> keywordTable = {
            // Variables
            "var", "const",
            
            // Declarations
            "fn", "struct", "record", "fnset", "interface", "enum", "union", "typedef",

            // Visibility Modifiers
            "public", "protected", "package_private", "file_private", "private",

            // FFI Modifiers
            "extern", "export",

            // Attibutes Modifiers
            "packed", "partial", "abstract",

            // Namespacing
            "package", "use",

            // Flow Control
            "if", "else", "while", "switch",
            "return", "break", "continue",

            // Builtin Constants
            "nullval", "nullptr",
            "true", "false",

            // Builtin Types
            "any", "void",

            // Inline Languages
            "asm",
        };

        /// @brief A simple string view containing non combinable symbols
        static constexpr std::string_view soloSymbols = "(){}[];$@,";

        /// @brief A simple string view containing combinable symbols
        static constexpr std::string_view combinableSymbols = "+-/*%=<>&|~!^?.:";

        /// @brief Asserts that the given entry is present in the given STL container
        /// @tparam TKey The entry type
        /// @tparam TContainer The Container Type
        /// @param entry The reference of the entry to look for
        /// @param cont The STL container to look in 
        template<typename TKey, typename TContainer>
        [[nodiscard]] static bool isEntryIn(const TKey entry, const TContainer& cont) {
            return std::find(std::begin(cont), std::end(cont), entry) != std::end(cont);
        }

        /// @brief Asserts that the given entry is present in the given STL container (pass by reference)
        /// @tparam TKey The entry type
        /// @tparam TContainer The Container Type
        /// @param entry The reference of the entry to look for
        /// @param cont The STL container to look in 
        template<typename TKey, typename TContainer>
        [[nodiscard]] static bool isEntryRefIn(const TKey& entry, const TContainer& cont) {
            return std::find(std::begin(cont), std::end(cont), entry) != std::end(cont);
        }
    };

    /// @brief Tokenizes a source code in order for it to be processed furthermore
    export class Lexer {
    private:
        /// @brief A token represents a value in the source code, with more informations such as the token type (number, string, etc) and its offset in the text
        struct Token { 
            /// @brief Enumerates every possibility of what a token can be
            enum class TokenType { 
                // Generic
                _EOF,       // End of file token
                _INVALID,   // Invalid token

                // Structural
                _LITERAL,    // Literal token (a constant value)
                _NUMBER,     // A number (integer or floating point)
                _CHAR,       // Text between ' '
                _STRING,     // Text between " "
                _IDENTIFIER, // A name in the code (variable, function, you name it)
                _KEYWORD,    // Specific behaviour coupled with names (e.g "return", "if")
                _SYMBOL,     // Non alpha characters (Such as '{', '}', etc..)
            };

            /// @brief The token type
            TokenType type;

            /// @brief The token value
            std::string_view lexeme;
            
            /// @brief The token offset relative to the beggining of the source code
            std::size_t offset;

            /// @brief Allows for std::cout to print token informations
            friend std::ostream& operator <<(std::ostream& os, const Token& token) {
                os << "[TYPE : " 
                   << static_cast<int>(token.type) 
                   << "] | [VALUE : "
                   << token.lexeme
                   << "]";

                return os;
            }
        };

        iterableBuffer<char> sourceBuffer;
        iterableBuffer<Token> internalBuffer;
        
        /// @brief Flushes the character buffer and the Token buffer of the preprocessor
        void flushBuffers() noexcept;

        // == Processing Utilites ==

        /// @brief Asserts that the given character is any entry of [a-zA-Z]
        /// @param c The character to check
        [[nodiscard]] constexpr bool isLetter(const char c) noexcept;

        /// @brief Asserts that the given character is any entry of [0-9]
        /// @param c The character to check
        [[nodiscard]] constexpr bool isDigit(const char c) noexcept;

        /// @brief Asserts that the given character is a symbol that cannot be combined with others
        /// @param c The character to check
        [[nodiscard]] constexpr bool isSimpleSymbol(const char c) noexcept;

        /// @brief Asserts that the given character is a symbol that can be combined with others
        /// @param c The character to check
        [[nodiscard]] constexpr bool isCombinableSymbol(const char c) noexcept;

        /// @brief Asserts that the given character is a generic symbol
        /// @param c The character to check 
        [[nodiscard]] constexpr bool isGenericSymbol(const char c) noexcept;

        /// @brief Creates a token containing a word
        [[nodiscard]] Token createWordToken() noexcept;

        /// @brief Creates a token containing a number
        [[nodiscard]] Token createNumberToken() noexcept;

        /// @brief Creates a token containing a symbol
        [[nodiscard]] Token createSymbolToken() noexcept;

        /// @brief Creates a token containing a textual constant (e.g : string or char) 
        [[nodiscard]] Token createTextualConstantToken() noexcept;

    public:
        /// @brief A lexer payload, contains the processed token buffer for the parser to process
        struct LexerOutput {
            iterableBuffer<Token> outputTokens;
        };

        // == Processing ==

        /// @brief Tokenizes a preprocessor payload and fills in the buffers of the lexer
        /// @param inputPayload The preprocessor output to process
        void process(Preprocessor::PreprocessorOutput inputPayload);

        /// @brief Transfers the internal buffer over to a payload and flushes the internal buffers making the unit ready for a new stream
        LexerOutput buildOutput();
    };
}