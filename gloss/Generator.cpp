#include "Generator.hpp"

static void writeEnums(const LanguageFile& langFile, const Generator::Options& options, std::ostream& stream)
{
    const std::string& TAB = options.tabSymbol;

    // write language codes
    stream << "\tenum class Lang : std::size_t\n" << TAB << "{\n";
    for(const auto& langName : langFile.languages)
    {
        stream << TAB << TAB << langName << ",\n";
    }
    stream << TAB <<"};\n";

    stream << TAB << "[[maybe_unused]] static constexpr std::size_t NUM_LANGUAGES = " << langFile.languages.size() << ";\n\n";

    // write symbol codes
    stream << TAB << "enum class Symbol : std::size_t\n" << TAB << "{\n";
    for(const auto&[symbol, translations] : langFile.symbols)
    {
        stream << TAB << TAB << symbol << ",\n";
    }
    stream << TAB << "};\n";

    stream << TAB << "[[maybe_unused]] static constexpr std::size_t NUM_SYMBOLS = " << langFile.symbols.size() << ";\n\n";
}

static void writeLanguageTranslations(std::size_t langIndex, const LanguageFile& langFile, const Generator::Options& options, std::ostream& stream)
{
    const std::string& TAB = options.tabSymbol;

    stream << TAB << TAB << "std::array<const char*," << langFile.symbols.size() << ">{\n";
    for(const auto&[symbol, translations] : langFile.symbols)
    {
        stream << TAB << TAB << TAB << "\"" << translations.at(langIndex) << "\",\n";
    }
    stream << TAB << TAB << "},\n";
}

void Generator::writeHeader(const LanguageFile& langFile, const Generator::Options& options, std::ostream& stream)
{
    const std::string& TAB = options.tabSymbol;

    stream << "#pragma once\n\n";
    stream << "#include <string_view>\n\n";
    stream << "namespace " << options.translatorName << "\n{\n";

    writeEnums(langFile, options, stream);

    // output getter function declaration
    stream << TAB << "const std::string_view getText(Lang, Symbol);\n";

    if(options.includeEnumStringGetters)
    {
        // output symbol getter function declarations
        stream << TAB << "const std::string_view getSymbolAsString(Symbol);\n";
        stream << TAB << "const std::string_view getLanguageAsString(Lang);\n";
    }

    stream << "}\n";
}

void Generator::writeSource(const LanguageFile& langFile, const Generator::Options& options, std::ostream& stream)
{
    const std::string& TAB = options.tabSymbol;

    // write prelude
    stream << "#include <array>\n";
    stream << "#include <string_view>\n\n";
    stream << "namespace " << options.translatorName << "\n{\n"; 

    // write symbol and language enums
    writeEnums(langFile, options, stream);

    // write translation table
    stream << TAB << "static const std::array<std::array<const char*," << langFile.symbols.size() << ">, " << langFile.languages.size() << "> s_translations {\n"; 
    for(std::size_t i = 0; i < langFile.languages.size(); i++)
    {
        writeLanguageTranslations(i, langFile, options, stream);
    }
    stream << TAB << "};\n\n";

    // write translation getter implementation
    stream << TAB << "const std::string_view getText(Lang l, Symbol s)\n";
    stream << TAB << "{\n";
    stream << TAB << TAB << "return s_translations.at(static_cast<std::size_t>(l)).at(static_cast<std::size_t>(s));\n";
    stream << TAB << "}\n";

    if(options.includeEnumStringGetters)
    {
        // write language names
        stream << TAB << "static const std::array<const char*," << langFile.languages.size() << "> s_languageNames {\n"; 
        for(const auto& lang : langFile.languages)
        {
            stream << TAB << TAB << "\"" << lang << "\",\n";
        }
        stream << TAB << "};\n";

        // write symbol names
        stream << TAB << "static const std::array<const char*," << langFile.symbols.size() << "> s_symbolNames {\n"; 
        for(const auto&[symbol, translations] : langFile.symbols)
        {
            stream << TAB << TAB << "\"" << symbol << "\",\n";
        }
        stream << TAB << "};\n";

        // write implementation for symbol name getter
        stream << TAB << "const std::string_view getSymbolAsString(Symbol s)\n";
        stream << TAB << "{\n";
        stream << TAB << TAB << "return s_symbolNames.at(static_cast<std::size_t>(s));\n";
        stream << TAB << "}\n\n";

        // write implementation for language name getter
        stream << TAB << "const std::string_view getLanguageAsString(Lang l)\n";
        stream << TAB << "{\n";
        stream << TAB << TAB << "return s_languageNames.at(static_cast<std::size_t>(l));\n";
        stream << TAB << "}\n";
    }

    stream << "}\n";
}