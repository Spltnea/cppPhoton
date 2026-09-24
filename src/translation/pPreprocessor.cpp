#include "translation/pPreprocessor.hpp"

namespace photon {
#pragma region Private Methods

    std::string pPreprocessor::stripComments(const std::string& src) {
        std::string out;
        out.reserve(src.size());

        enum class State { NORMAL, INLINE_COMMENT, BLOCK_COMMENT, STRING, CHAR };
        State state = State::NORMAL;
        bool isEscaped = false;

        for (size_t i = 0; i < src.size(); ++i) {
            char c = src[i];
            char next = (i + 1 < src.size()) ? src[i + 1] : '\0';

            switch (state) {
                case State::NORMAL:
                    if (c == '"') {
                        state = State::STRING;
                        isEscaped = false;
                        out.push_back(c);
                    } else if (c == '\'') {
                        state = State::CHAR;
                        isEscaped = false;
                        out.push_back(c);
                    } else if (c == '/' && next == '/') {
                        state = State::INLINE_COMMENT;
                        ++i; // skip '/'
                    } else if (c == '/' && next == '*') {
                        state = State::BLOCK_COMMENT;
                        ++i; // skip '*'
                    } else {
                        out.push_back(c);
                    }
                    break;

                case State::STRING:
                case State::CHAR: {
                    out.push_back(c);
                    char quote = (state == State::STRING) ? '"' : '\'';
                    if (isEscaped) {
                        isEscaped = false;
                    } else if (c == '\\') {
                        isEscaped = true;
                    } else if (c == quote) {
                        state = State::NORMAL;
                    }
                    break;
                }

                case State::INLINE_COMMENT:
                    if (c == '\n') {
                        state = State::NORMAL;
                        out.push_back(c);
                    }
                    break;

                case State::BLOCK_COMMENT:
                    if (c == '*' && next == '/') {
                        state = State::NORMAL;
                        ++i; // skip '/'
                    } else if (c == '\n') {
                        out.push_back(c); // Preserve newlines for compiler error line numbers
                    }
                    break;
            }
        }

        return out;
    }

#pragma endregion Private Methods

#pragma region Public Methods

    void pPreprocessor::applyPreprocessorPass(std::string sourceFileName) {
        fs::path filePath(sourceFileName);

        // checks if the file exists and is regular file
        if (!fs::exists(filePath)) {
            throw std::runtime_error("[PREPROCESSOR] - File does not exist: " + sourceFileName);
        }

        if (!fs::is_regular_file(filePath)) {
            throw std::runtime_error("[PREPROCESSOR] - Path is not a regular file: " + sourceFileName);
        }

        // Fetch file size and handle empty files
        auto fileSize = fs::file_size(filePath);
        if (fileSize == 0) { processedFileBuffer = std::make_unique<iterableBuffer<char>>(); return; }
        
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) { throw std::runtime_error("[PREPROCESSOR] - Failed to open file: " + sourceFileName); }

        std::string rawCnt;
        rawCnt.resize(fileSize);

        file.read(&rawCnt[0], fileSize);
        file.close();

        std::string cleanedCnt = stripComments(rawCnt);
        processedFileBuffer = std::make_unique<iterableBuffer<char>>(std::move(cleanedCnt));
    }

#pragma enderegion Public Methods
    
} // namespace photon