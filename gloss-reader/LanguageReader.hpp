#pragma once
#include <istream>
#include <stdexcept>

#include "StringViewScanner.hpp"
#include "LanguageFile.hpp"

struct LanguageReaderError : public std::runtime_error
{
    LanguageReaderError(const std::string& message) : 
        std::runtime_error(message)
    {
    }
};

namespace LanguageFileReader
{
    LanguageFile read(std::istream& stream);
}