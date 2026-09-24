#ifndef P_COMPILER_HPP
#   define P_COMPILER_HPP

#include <memory>

#include "pProject.hpp"
#include "pCLI.hpp"

#include "translation/pPreprocessor.hpp"
#include "translation/pLexer.hpp"
#include "translation/pParser.hpp"


namespace photon {

    /** The compiler object, handles all the pipelining that transforms a photon project into an executable */
    class pCompiler {
    private:
        std::unique_ptr<pPreprocessor>  preprocessorModule;
        std::unique_ptr<pLexer>         lexerModule;
        std::unique_ptr<pParser>        parserModule;
        //std::unique_ptr<pLLVMGenerator> generatorModule;
    
    public:
        pCompiler(std::unique_ptr<pPreprocessor> preprocessor, std::unique_ptr<pLexer> lexer, std::unique_ptr<pParser> parser)
            : preprocessorModule(std::move(preprocessor)), lexerModule(std::move(lexer)), parserModule(std::move(parser)) {}

        void compile(Project& project, std::string executableName, std::string outputDirectory);
    };

}

#endif