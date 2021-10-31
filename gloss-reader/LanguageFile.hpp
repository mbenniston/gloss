#pragma once
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

struct LanguageMergeError : public std::runtime_error
{
    LanguageMergeError(const std::string& message) : 
        std::runtime_error(message)
    {
    }
};

// contains the interpreted contents of a language file
// including:
//  - a list of all the languages provided in the file
//  - a map of each symbol to its translations in the given languages
struct LanguageFile
{
    std::vector<std::string> languages;
    std::map<std::string, std::vector<std::string>> symbols;

    // combines two languages files with differing languages but the same symbol set together
    void merge(const LanguageFile& other);
};
