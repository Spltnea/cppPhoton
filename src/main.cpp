#include <iostream>

#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

int main(void)
{
    std::cout << "Hello World !\n";

    llvm::LLVMContext context;
    llvm::Module module("cppPhoton", context);

    return 0;
}