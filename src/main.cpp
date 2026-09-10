#include <iostream>

#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "translation/pPreprocessor.hpp"
#include "translation/pLexer.hpp"

int main(int argc, char** argv)
{
    // get files
    if (argc < 2) {
        std::cerr << "Usage : photon build <filename.pho>\n" 
                  << "Or : photon help <command | *>\n";

        return 1;
    }

    if (argv[0] == "build") {
        std::string fileName = argv[2];
    }

    if (argv[0] == "help") {
        // simply stop for now
        return 0;
    }
    
    /*
        Init compilation pipeline 
        A : Preprocessing Pass => transforms a .pho file to a .ppf file 
        B : Lexing Pass => transforms a .ppf file to a stream of tokens of type tokenVec_t  
    */

    // Preprocessor init
    photon::pPreprocessor preprocessor(argv[1]);
    photon::PreprocessResult preprocessingResult = preprocessor.applyPreprocessorPass();

    if (!preprocessingResult.success) {
        std::cerr << "Compilation Errors Occured !\n";
        return 1;
    }

    // Lexer init
    photon::pLexer lexer(preprocessingResult);
    lexer.applyLexerPass();

    photon::iterableBuffer tempArray = lexer.getTokens();
    for (auto tok : tempArray) {
        std::cout   << "{TYPE : " << static_cast<int>(tok.type) 
                    << ", AT : {" << tok.lineNo << ", " << tok.colNo << "}"
                    << ", LEXEME : " << tok.lexeme << "} \n";
    }

    // Init llvm modules
    llvm::LLVMContext context;
    llvm::Module module("cppPhoton", context);

    return 0;
}