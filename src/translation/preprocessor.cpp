#include "translation/preprocessor.hpp"

namespace photon {
#pragma region Private Methods

    void Preprocessor::flushBuffers() {
        Preprocessor::rawSource.clear();
        Preprocessor::internalBuffer.clear();
    }

    // AI Generated (internal algorithm) then verified using documentation

    void Preprocessor::stripComments(std::string& rawInput) {
        // The regex rule used to strip comments, has multiple groups for better comment stripping
        static const std::regex tokenRegex(
            R"regex((R"([^(\\\s]{0,16})\([\s\S]*?\)\2"|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')|(/\*[\s\S]*?\*/)|(//[^\r\n]*))regex"
        );

        std::string result;
        result.reserve(rawInput.size());

        auto begin = std::sregex_iterator(rawInput.begin(), rawInput.end(), tokenRegex);
        auto end = std::sregex_iterator();
        size_t lastPos = 0;

        for (auto it = begin; it != end; ++it) {
            const auto& match = *it;

            result.append(rawInput, lastPos, match.position() - lastPos);

            if (match[1].matched) {
                result.append(match[1].first, match[1].second);
            } else if (match[3].matched) {
                result += ' ';
                for (char ch : match[3].str()) {
                    if (ch == '\n') result += '\n';
                }
            }

            lastPos = match.position() + match.length();
        }

        result.append(rawInput, lastPos, rawInput.size() - lastPos);
        rawInput = std::move(result);
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
            std::println("File [{}] doesn't have the required extension, ignoring", filePath.string());
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

        // Buf allocation
        rawSource.resize(fileSize);
        ifs.read(rawSource.data(), static_cast<std::streamsize>(fileSize));

        stripComments(rawSource);

        // char vector allocation
        internalBuffer.append(rawSource);
    }

#pragma endregion
}