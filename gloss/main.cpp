#include <iostream>
#include <fstream>

#include "argparse_nowarn.hpp"

#include "LanguageReader.hpp"
#include "Generator.hpp"

int main(int argc, char** argv)
{
    argparse::ArgumentParser program{ "gloss" };

    program.add_argument("--source");
    program.add_argument("--header");
    program.add_argument("--translator_name")
        .default_value(std::string("Translator"));

    program.add_argument("--tab_symbol")
        .default_value(std::string("\t"));

    program.add_argument("-e", "--include_enum_getters")
        .default_value(false)
        .implicit_value(true);

    program.add_argument("translation_files")
        .remaining()
        .default_value(std::vector<std::string>{});

    try
    {
        program.parse_args(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << program;

        return 1;
    }

    auto files = program.get<std::vector<std::string>>("translation_files");
    
    if(files.empty())
    {
        std::cout << "no files provided!" << std::endl;
        return 1;
    }

    std::optional<LanguageFile> mergedFile;

    for (auto& file : files)
    {
        std::ifstream inStream { file };
        try
        {
            LanguageFile newLang = LanguageFileReader::read(inStream);
            if(!mergedFile)
            {
                mergedFile = newLang;
            }
            else
            {
                mergedFile->merge(newLang);
            }
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "Occurred when merging file: \"" << file << "\"\n";
            return 1;
        }
    }

    Generator::Options options;
    options.translatorName = program.get("--translator_name");
    options.includeEnumStringGetters = program.get<bool>("--include_enum_getters");
    options.tabSymbol = program.get("--tab_symbol");

    if(program.is_used("--header"))
    {
        std::ofstream outHeader { program.get("--header") };
        Generator::writeHeader(*mergedFile, options, outHeader);
    }

    if(program.is_used("--source"))
    {
        std::ofstream outSource { program.get("--source") };
        Generator::writeSource(*mergedFile, options, outSource);
    }
    
    return 0;
}