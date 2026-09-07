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

        std::string buildOutputFileName() const {
            size_t lastDot = sourceFileName.find_last_of(".");
            if (lastDot == std::string::npos) return sourceFileName + "." + processedFileExt;
            return sourceFileName.substr(0, lastDot) + "." + processedFileExt;
        }

        // Helper to strip comments from the input file and send the processed file to the destination file reference
        // Note : Has a small algorithm to detect if those comments delimiters are present in strings to not strip them
        void stripComments(std::ifstream& inputFile, std::ofstream& destinationFile) {
            char c;
            bool inLineComment = false;
            bool inBlockComment = false;
            bool inString = false;
            bool isEscaped = false;

            while (inputFile.get(c)) {
                if (inString) {
                    destinationFile.put(c);
                    if (c == '\\' && !isEscaped) {
                        isEscaped = true;
                    } else {
                        if (c == '"' && !isEscaped) {
                            inString = false;
                        }
                        isEscaped = false;
                    }
                    continue;
                }

                if (inLineComment) {
                    if (c == '\n') {
                        inLineComment = false;
                        destinationFile.put(c);
                    }
                    continue;
                }

                if (inBlockComment) {
                    if (c == '*' && inputFile.peek() == '/') {
                        inputFile.get(c); // Consomme le '/'
                        inBlockComment = false;
                    } else if (c == '\n') {
                        destinationFile.put(c);
                    }
                    continue;
                }

                if (c == '"') {
                    inString = true;
                    isEscaped = false;
                    destinationFile.put(c);
                }
                else if (c == '/' && inputFile.peek() == '/') {
                    inLineComment = true;
                }
                else if (c == '/' && inputFile.peek() == '*') {
                    inBlockComment = true;
                }
                else {
                    destinationFile.put(c);
                }
            }
        }

    public:
        explicit pPreprocessor(std::string fileName) : sourceFileName(std::move(fileName)) {}
        
        /** Applies a preprocessor pass to the source file and builds a clean source code */
        PreprocessResult applyPreprocessorPass() {
            PreprocessResult result;
            result.sourcePath = sourceFileName;
            result.processedFilePath = buildOutputFileName();

            std::ifstream inputFile(sourceFileName);
            if (!inputFile.is_open()) {
                auto logFrame = pLogger::buildFrame(IdPrefix::PREPROCESSOR_LOG, 
                                                    SeverityPrefix::ERR, 0x01, 
                                                    {"Cannot open file : \"", sourceFileName, "\" as file does not exist at specified location"});
                pLogger::lprint(logFrame);
                return result;
            }

            std::ofstream outputFile(result.processedFilePath);
            if (!outputFile.is_open()) {
                auto logFrame = pLogger::buildFrame(IdPrefix::PREPROCESSOR_LOG, 
                                                    SeverityPrefix::ERR, 0x02, 
                                                    {"Cannot create file : \"", result.processedFilePath, "\""});
                pLogger::lprint(logFrame);
                return result;
            }

            stripComments(inputFile, outputFile);

            result.success = true;

            auto logFrame = pLogger::buildFrame(IdPrefix::PREPROCESSOR_LOG, 
                                                SeverityPrefix::INFO, 0x01, 
                                                {"Preprocessor pass done on file \"", sourceFileName, "\" and sent it to \"", result.processedFilePath, "\""});
            pLogger::lprint(logFrame);

            return result;
        }
    };
}

#endif