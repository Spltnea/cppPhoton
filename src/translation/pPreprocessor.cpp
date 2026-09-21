#include "translation/pPreprocessor.hpp"

namespace photon {
#pragma region Private Methods

    std::string pPreprocessor::buildOutputFileName() const {
        size_t lastDot = sourceFileName.find_last_of(".");
        if (lastDot == std::string::npos) return sourceFileName + "." + processedFileExt;
        return sourceFileName.substr(0, lastDot) + "." + processedFileExt;
    }

    void pPreprocessor::stripComments(std::ifstream& inputFile, std::ofstream& destinationFile) {
        enum class State {
            Normal,
            InLineComment,
            InBlockComment,
            InString,
            InChar,
        };

        State state = State::Normal;
        bool isEscaped = false;
        std::string rawDelimiter;
        char c;

        while (inputFile.get(c)) {
            switch (state) {

                case State::Normal: {
                    if (c == '\'') {
                        state = State::InChar;
                        isEscaped = false;
                        destinationFile.put(c);
                    }
                    else if (c == '"') {
                        state = State::InString;
                        isEscaped = false;
                        destinationFile.put(c);
                    }
                    else if (c == '/' && inputFile.peek() == '/') {
                        inputFile.get(c);
                        state = State::InLineComment;
                    }
                    else if (c == '/' && inputFile.peek() == '*') {
                        inputFile.get(c);
                        state = State::InBlockComment;
                    }
                    else { destinationFile.put(c); }
                    break;
                }

                case State::InChar: {
                    destinationFile.put(c);
                    if (isEscaped) { isEscaped = false; }
                    else if (c == '\\') { isEscaped = true; }
                    else if (c == '\'') { state = State::Normal; }
                    break;
                }

                case State::InString: {
                    destinationFile.put(c);
                    if (isEscaped) { isEscaped = false; }
                    else if (c == '\\') { isEscaped = true; }
                    else if (c == '"') { state = State::Normal; }
                    break;
                }

                case State::InLineComment: {
                    if (c == '\n') {
                        state = State::Normal;
                        destinationFile.put(c);
                    }
                    break;
                }

                case State::InBlockComment: {
                    if (c == '*' && inputFile.peek() == '/') {
                        inputFile.get(c);
                        state = State::Normal;
                    } else if (c == '\n') { destinationFile.put(c); }
                    break;
                }
            }
        }
    }

#pragma endregion Private Methods

#pragma region Public Methods

    PreprocessResult pPreprocessor::applyPreprocessorPass() {
        PreprocessResult result;
        result.sourcePath = sourceFileName;
        result.processedFilePath = buildOutputFileName();

        std::ifstream inputFile(sourceFileName);
        if (!inputFile.is_open()) {
            auto logFrame = pLogger::buildFrame(
                IdPrefix::PREPROCESSOR_LOG, 
                SeverityPrefix::ERR, 0x01, 
                {"Cannot open file : \"", sourceFileName, "\" as file does not exist at specified location"}
            );
            pLogger::lprint(logFrame);
            return result;
        }

        std::ofstream outputFile(result.processedFilePath);
        if (!outputFile.is_open()) {
            auto logFrame = pLogger::buildFrame(
                IdPrefix::PREPROCESSOR_LOG, 
                SeverityPrefix::ERR, 0x02, 
                {"Cannot create file : \"", result.processedFilePath, "\""}
            );
            pLogger::lprint(logFrame);
            return result;
        }

        stripComments(inputFile, outputFile);

        result.success = true;

        auto logFrame = pLogger::buildFrame(
            IdPrefix::PREPROCESSOR_LOG, 
            SeverityPrefix::INFO, 0x01,                   
            {"Preprocessor pass done on file \"", sourceFileName, "\" and sent it to \"", result.processedFilePath, "\""}
        );

        pLogger::lprint(logFrame);

        return result;
    }

#pragma enderegion Public Methods
    
} // namespace photon