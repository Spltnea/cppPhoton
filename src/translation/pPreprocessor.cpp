#include "translation/pPreprocessor.hpp"

namespace photon {
    PreprocessResult pPreprocessor::applyPreprocessorPass() {
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
} // namespace photon