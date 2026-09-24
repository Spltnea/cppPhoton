#ifndef P_PROJECT_HPP
#   define P_PROJECT_HPP

#include <string>
#include <memory>
#include <filesystem>

#include "translation/intermediateObjects/pSymbol.hpp"
#include "utils/types/iterableBuffer.hpp"
#include "utils/toml.hpp"

namespace photon {
    
    namespace fs = std::filesystem;

    /**
     *  A project is a collection of source files destined to create an executable program
     *  A project is composed of :
     *  - Files of type @c PhotonSourceFile , they hold the source code of the project
     *  - The global package which stores every symbol declaration
     *  - A build configuration [in TOML format]
     *  The project can get symbols from the parser which are then validated by the semantic analyzer
     *  Upon finishing analysis, the project is sent file by file to LLVM for compiling and assembling, resulting in a working file
     *  A project is created using the command "photon init --name <projectName> --standard <version>"
     */
    class Project {
    private :
        // Basic project informations
        std::string projectName;
        unsigned int projectVersion;
        unsigned int coreLibStd;

        // A basic source file structure
        struct PhotonSourceFile  { std::string filePath; std::string contents; };

        // A basic configuration file structure
        struct ConfigurationFile { std::string filePath; std::string contents; };

        // An iterable collection of source files in the project
        std::unique_ptr<iterableBuffer<PhotonSourceFile>> projectFiles;

        // An iterable collection of configuration files in the project
        std::unique_ptr<iterableBuffer<ConfigurationFile>> configurationFiles;
        
        // The global package which stores every symbol
        Symbols::PackageSymbol globalPackage;

    public:
        Project(std::string name, unsigned int version, unsigned int standard) 
            : projectName(std::move(name)), projectVersion(version), coreLibStd(standard), globalPackage("GLOBAL_PKG", {}, Symbols::BaseSymbol::Attributes::GLOBAL_ATTR) {}
        
        inline const std::string& getProjectName() {

        }
        
        inline const unsigned int getProjectVersion() {

        }

        inline const unsigned int getProjectCoreLibStandard() {

        }

        inline const iterableBuffer<PhotonSourceFile>& getSourceFiles() {
            return *projectFiles;
        }

        /** Asserts that the given source file has the correct extension */
        inline const bool fileHasExtension(const PhotonSourceFile& file, std::string expectedExtension) {
            fs::path filePath = file.filePath;
            return filePath.extension() == expectedExtension;
        }

        PhotonSourceFile createSourceFile(std::string name);
        ConfigurationFile createConfigurationFile(std::string name);

        void appendSourceFile(PhotonSourceFile& sourceFile);
        void appendConfigurationFile(ConfigurationFile& configurationFile);

        void listSourceFiles();
        void listConfigurationsFiles();
    };
}

#endif