#include "LanguageReader.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string_view>
#include <optional>

static constexpr const char* const HEADER_SYMBOL = "symbol";

static constexpr bool isValidSymbolStartChar(char c)
{
    return std::isalpha(c);
}

static constexpr bool isValidSymbolInfixChar(char c)
{
    return std::isalnum(c) || c == '_';
}

static bool startswith(const std::string_view str, const std::string_view start, bool ignoreWhiteSpace = true)
{   
    auto strStart = str.begin();
    if(ignoreWhiteSpace)
    {
        while(strStart != str.end() && std::isspace(*strStart))
        {
            strStart++;
        }
    }

    std::string_view strCut(strStart, 
        static_cast<std::size_t>(std::distance(strStart, str.end())));

    return strCut.size() >= start.size() && strCut.substr(0, start.size()) == start;
}

static std::string parseSymbol(StringViewScanner& scanner)
{
    std::stringstream symbolBuilder;

    scanner.dropWhitespace();

    symbolBuilder << scanner.expect(isValidSymbolStartChar);
    std::optional<char> currentChar;
    while((currentChar = scanner.peek()).has_value())
    {
        if(*currentChar == ',' || *currentChar == ' ')
        {
            break;
        } 
        else if(isValidSymbolInfixChar(*currentChar))
        {
            symbolBuilder << *currentChar;
            scanner.advance();
        }
        else 
        {
            throw LanguageReaderError("Unexpcted character: \""+ 
                std::string({*currentChar}) +"\" parsing symbol");
        }
    }

    return symbolBuilder.str();
}

static void handleHeader(const std::string_view line, LanguageFile& file)
{
    if(!file.languages.empty())
    {
        throw LanguageReaderError("Already set language codes");
    }

    StringViewScanner scanner{ line };

    std::string startingSymbol = parseSymbol(scanner);
    if(startingSymbol != HEADER_SYMBOL)
    {
        throw LanguageReaderError("Incorrect header symbol, first column of first row should be \"" + 
            std::string(HEADER_SYMBOL) + "\"");
    }

    scanner.dropWhitespace();

    while(scanner.consume(','))
    {
        std::string langID = parseSymbol(scanner);
        if(std::count(file.languages.begin(), file.languages.end(), langID) > 0)
        {
            throw LanguageReaderError("Repeated language column \""+   
                langID +"\", language codes should be unique");
        }
        else
        {
            file.languages.push_back(langID);
        }

        scanner.dropWhitespace();
    }
}

static std::string parseTranslation(StringViewScanner& scanner)
{
    std::stringstream translationBuilder;

    scanner.dropWhitespace();
    scanner.expect('"');

    while(true)
    {
        std::optional<char> currentChar = scanner.consume();

        if(currentChar.has_value())
        {
            if(*currentChar == '"')
            {
                break;
            } 
            else 
            {
                translationBuilder << *currentChar;
            }
        }
        else
        {
            throw LanguageReaderError("Expected to close \" instead got: end of line");
        }
    }

    return translationBuilder.str();
}

static void handleTranslation(const std::string_view line, LanguageFile& file)
{
    if(file.languages.empty())
    {
        throw LanguageReaderError("Language codes not set, these must be set in the first row to"
            "identify what languages the column translations correspond to");
    }

    StringViewScanner scanner{ line };
    std::vector<std::string> translations;
    translations.reserve(file.languages.size());

    std::string symbol = parseSymbol(scanner);

    scanner.dropWhitespace();
    
    while(scanner.consume(','))
    {
        translations.emplace_back(parseTranslation(scanner));
        scanner.dropWhitespace();
    }

    if(translations.size() != file.languages.size())
    {
        throw LanguageReaderError("Columns recieved do not match the number of language codes!");
    }

    if(file.symbols.count(symbol) > 0)
    {
        throw LanguageReaderError("Symbol: \""+ symbol +"\" already exists");
    }
    else
    {
        file.symbols.emplace(std::move(symbol), std::move(translations));
    }
}

LanguageFile LanguageFileReader::read(std::istream& stream)
{
    LanguageFile file;
    std::string line;
    while(std::getline(stream, line))
    {
        if(startswith(line, HEADER_SYMBOL))
        {
            handleHeader(line, file);
        }
        else
        {
            handleTranslation(line, file);
        }
    }
    return file;
}
