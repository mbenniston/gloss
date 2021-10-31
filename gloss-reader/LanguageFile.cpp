#include "LanguageFile.hpp"
#include <algorithm>

void LanguageFile::merge(const LanguageFile& other)
{
    // check symbols match, order of symbols does not matter between files only within the same file
    for(const auto&[symbol, translation] : symbols)
    {
        if(other.symbols.count(symbol) == 0)
        {
            throw LanguageMergeError("Cannot merge files, symbol: \""+ 
                symbol +"\" missing from file to merge with");
        }
    }

    for(const auto&[symbol, translation] : other.symbols)
    {
        if(symbols.count(symbol) == 0)
        {
            throw LanguageMergeError("Cannot merge files, symbol: \""+ 
                symbol +"\" missing from file being merged");
        }
    }

    for(std::size_t langIndex = 0; langIndex < other.languages.size(); langIndex++)
    {
        const auto& lang = other.languages.at(langIndex);
        if(std::count(languages.begin(), languages.end(), lang) > 0)
        {
            throw LanguageMergeError("Cannot merge files, language \""+ 
                lang +"\" already exists in the first file");
        }
        else
        {
            languages.push_back(lang);
            for(const auto&[symbol, translations] : other.symbols)
            {
                if(symbols.count(symbol) > 0)
                {
                    auto& currentTranslations = symbols.at(symbol);
                    currentTranslations.push_back(translations.at(langIndex));
                }
                else
                {
                    throw LanguageMergeError("Cannot merge files, the symbol \"" + 
                        symbol +  "\" does not exist file to merge with");
                }
            }
        }
    }
}