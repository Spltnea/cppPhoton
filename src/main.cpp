#include "translation/preprocessor.hpp"
#include "translation/lexer.hpp"

using namespace photon;

int main() {
    // Test file
    fs::path filePath = fs::path("sandbox") / "testProject" / "src" / "main.pho";
    fs::path absolutePath = fs::absolute(filePath);

    // Compilation pipeline
    Preprocessor preprocessor; // Preprocessor init
    Lexer lexer;               // Lexer init

    // == Preprocessor Processing ==
    preprocessor.process(absolutePath);
    auto preproc_output = preprocessor.buildOutput();
    
    // == Lexer Processing ==
    lexer.process(preproc_output);
    auto lexer_output = lexer.buildOutput();
    
    // == Parser Processing ==

    #ifdef DEBUG
    
        size_t tokenID = 0;
        size_t nodeID = 0;

        std::cout << "Lexer Contents : \n";
        for (auto item : lexer_output.outputTokens) {
            std::cout << "[" << tokenID << "]" << item << '\n';
            tokenID++;
        }
        std::cout << "Total Lexer Items : " << lexer_output.outputTokens.size();

    #endif // DEBUG
    return 0;
}