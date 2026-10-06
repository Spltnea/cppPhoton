export module Preprocessor;

import std;
import IterableBuffer;

namespace fs = std::filesystem;

namespace photon {
    /// @brief Handles copy pasting functions, clears comments and yields a cleaned source code
    export class Preprocessor {
    private:
        std::string rawSource;                 // The source code contained in the file as a contiguous array of memory
        iterableBuffer<char> internalBuffer;   // The internal char buffer to be transfered over
        
        /// @brief Flushes the string buffer and the character buffer of the preprocessor
        void flushBuffers();

        /// @brief Strips comments from the raw input
        /// @param rawInput The raw source code
        void stripComments(std::string& rawInput);

    public:
        /// @brief A preprocessor payload, contains the processed char buffer for the lexer to process
        struct PreprocessorOutput {
            iterableBuffer<char> outputBuffer;
        };
        
        /// @brief Processes the given file and fills in the internal output buffer
        /// @param filePath 
        void process(const fs::path& filePath);

        /// @brief Transfers the internal buffer over to a payload and flushes the internal buffers making the unit ready for a new file
        PreprocessorOutput buildOutput();  
    };
}