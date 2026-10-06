module Preprocessor;
import std;

namespace photon {
#pragma region Private Methods

    void Preprocessor::flushBuffers() {
        Preprocessor::rawSource.clear();
        Preprocessor::internalBuffer.clear();
    }

    // AI Generated (internal algorithm)

    void Preprocessor::stripComments(std::string& src) {
        enum class State {
            _IN_CODE,
            _IN_STRING,
            _IN_CHAR,
            _IN_LCOMMENT,
            _IN_BCOMMENT,
        };

        State state = State::_IN_CODE;
        bool escape = false;

        for (std::size_t i = 0; i < src.size(); ++i) {
            char c = src[i];
            char next = (i + 1 < src.size()) ? src[i + 1] : '\0';

            switch (state) {
                case State::_IN_CODE:
                    if (c == '"') {
                        state = State::_IN_STRING;
                    } else if (c == '\'') {
                        state = State::_IN_CHAR;
                    } else if (c == '/' && next == '/') {
                        state = State::_IN_LCOMMENT;
                        src[i] = ' ';
                    } else if (c == '/' && next == '*') {
                        state = State::_IN_BCOMMENT;
                        src[i] = ' ';
                    }
                    break;

                case State::_IN_STRING:
                    if (c == '\\' && !escape) {
                        escape = true;
                    } else {
                        if (c == '"' && !escape) state = State::_IN_CODE;
                        escape = false;
                    }
                    break;

                case State::_IN_CHAR:
                    if (c == '\\' && !escape) {
                        escape = true;
                    } else {
                        if (c == '\'' && !escape) state = State::_IN_CODE;
                        escape = false;
                    }
                    break;

                case State::_IN_LCOMMENT:
                    if (c == '\n') {
                        state = State::_IN_CODE;
                    } else {
                        src[i] = ' ';
                    }
                    break;

                case State::_IN_BCOMMENT:
                    if (c == '*' && next == '/') {
                        src[i] = ' ';
                        src[i + 1] = ' ';
                        ++i;
                        state = State::_IN_CODE;
                    } else if (c != '\n') {
                        src[i] = ' ';
                    }
                    break;
            }
        }
    }

#pragma endregion

#pragma region Public Methods

    Preprocessor::PreprocessorOutput Preprocessor::buildOutput() {
        PreprocessorOutput output = { std::move(internalBuffer) };
        flushBuffers();

        return output;
    }

    void Preprocessor::process(const fs::path& filePath) {
        if (!fs::exists(filePath)) {
            throw std::runtime_error("File [" + filePath.string() + "] does not exist");
        }

        if (filePath.extension() != ".pho") {
            throw std::runtime_error("File [" + filePath.string() + "] doesn't have the required extension");
            return;
        }

        if (!fs::is_regular_file(filePath)) {
            throw std::runtime_error("File [" + filePath.string() + "] is not a regular file");
        }

        const auto fileSize = fs::file_size(filePath);

        std::ifstream ifs(filePath, std::ios::binary);
        if (!ifs) {
            throw std::runtime_error("Failed to open file [" + filePath.string() + "]");
        }

        rawSource.resize(fileSize);
        ifs.read(rawSource.data(), static_cast<std::streamsize>(fileSize));

        stripComments(rawSource);

        internalBuffer.append(rawSource);
    }

#pragma endregion
}