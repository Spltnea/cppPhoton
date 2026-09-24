#include "compilation/pCompiler.hpp"

namespace photon {
#pragma region Public Members

    void pCompiler::compile(Project& project, std::string executableName, std::string outputDirectory) {
        const auto& sourceFiles = project.getSourceFiles();

        if (sourceFiles.isEmpty()) {
            throw std::runtime_error("There are no source files to parse");
        } 

        for (const auto& file : sourceFiles) {
            if (!project.fileHasExtension(file, ".pho")) {
                std::cout << "Ignored file (" << file.filePath << ") as it does not have a valid extension";
            }

            preprocessorModule->applyPreprocessorPass(file.filePath);
            lexerModule->applyLexerPass(preprocessorModule->getProcessedSource());
            parserModule->applyParserPass(lexerModule->getTokens());
        }
    }

#pragma endregion Private Members
}