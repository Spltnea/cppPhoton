#ifndef P_PREPROC_HPP
#   define P_PREPROC_HPP

#include <string>
#include <iostream>
#include <fstream>

#include "utils/pLogger.hpp"

namespace photon {

    struct PreprocessResult {
        bool success{false};
        std::string sourcePath;
        std::string processedFilePath;
    };

    /** Strips comments from a raw source code and performs copy paste actions */
    class pPreprocessor {
    private:
        const std::string processedFileExt = "ppf";
        std::string sourceFileName;

        // Helper to build the output file name
        std::string buildOutputFileName() const;

        // Helper to strip comments from the input file and send the processed file to the destination file reference
        // Note : Has a small algorithm to detect if those comments delimiters are present in strings to not strip them
        void stripComments(std::ifstream& inputFile, std::ofstream& destinationFile);

    public:
        explicit pPreprocessor(std::string fileName) : sourceFileName(std::move(fileName)) {}
        
        /** Applies a preprocessor pass to the source file and builds a clean source code */
        PreprocessResult applyPreprocessorPass();
        
    }; // pPreprocessor
} // namespace photon

#endif // P_PREPROC_HPP