#pragma once
#include <ostream>
#include "LanguageFile.hpp"

namespace Generator
{
    struct Options
    {
        std::string translatorName = "translator";
        std::string tabSymbol = "\t";
        bool includeEnumStringGetters = true;
    };

    void writeSource(const LanguageFile& langFile, 
        const Generator::Options& options, 
        std::ostream& stream);

    void writeHeader(const LanguageFile& langFile, 
        const Generator::Options& options, 
        std::ostream& stream);
}