#include "StringViewScanner.hpp"

#include <stdexcept>

bool StringViewScanner::atEnd() const 
{
    return m_index >= m_source.size();
}

std::optional<char> StringViewScanner::peek() const
{
    if(m_index < m_source.size())
    {
        return m_source.at(m_index);
    }

    return std::nullopt; 
}

std::optional<char> StringViewScanner::advance()
{
    m_index++;
    return peek();
}

std::optional<char> StringViewScanner::consume()
{
    auto currentChar = peek();
    if(currentChar.has_value())
    {
        advance();
    }

    return currentChar;
}

std::optional<char> StringViewScanner::consume(char c)
{
    auto currentChar = peek();
    if(currentChar.has_value() && *currentChar == c)
    {
        advance();
        return currentChar;
    }

    return std::nullopt;
}

std::optional<char> StringViewScanner::consume(std::function<bool(char)> fun)
{
    auto currentChar = peek();
    if(currentChar.has_value() && fun(*currentChar))
    {
        advance();
        return currentChar;
    }

    return std::nullopt;
}

char StringViewScanner::expect(char c)
{
    auto currentChar = peek();
    if(!currentChar.has_value())
    {
        throw LanguageScannerError("Expected character: \""+ std::string({c}) +"\", instead got: end of string");
    }

    if(*currentChar == c)
    {
        advance();
        return c;
    }
    else
    {
        throw LanguageScannerError("Expected character: \""+ std::string({c}) +"\", instead got: \"" + std::string({*currentChar}) +"\"");
    }
}

char StringViewScanner::expect(std::function<bool(char)> fun)
{
    auto currentChar = peek();
    if(!currentChar.has_value())
    {
        throw LanguageScannerError("Expected to satisfy functor, instead got: end of string");
    }

    if(fun(*currentChar))
    {
        advance();
        return *currentChar;
    }
    else
    {
        throw LanguageScannerError("Expected to satisfy functor, instead got: \"" 
            + std::string({*currentChar}) +"\"");
    }
}

void StringViewScanner::dropWhitespace()
{
    auto currentChar = peek();
    while(currentChar.has_value() && std::isspace(*currentChar))
    {
        advance();
        currentChar = peek();
    }
}

StringViewScanner::StringViewScanner(const std::string_view sv) : m_source{sv}, m_index{0}
{
}