#ifndef P_PREPROC_HPP
#   define P_PREPROC_HPP

#include <iostream>
#include <fstream>
#include <filesystem>

#include <stdexcept>

#include <string>
#include <memory>

#include "utils/types/iterableBuffer.hpp"

namespace photon {

    namespace fs = std::filesystem;

    /** Strips comments from a raw source code and performs copy paste actions */
    class pPreprocessor {
    private:
        std::string sourceCodeBuffer;
        std::unique_ptr<iterableBuffer<char>> processedFileBuffer;

        // Strips comments from the source file buffer
        std::string stripComments(const std::string& src);

    public:
        /** Applies a preprocessor pass to the source file and builds a clean source code */
        void applyPreprocessorPass(std::string sourceFileName);
        
        /** Returns a reference to the internal preprocessor's result buffer */
        inline iterableBuffer<char>& getProcessedSource() {
            return *processedFileBuffer;
        }

    }; // pPreprocessor
} // namespace photon

#endif // P_PREPROC_HPP