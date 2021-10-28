#pragma once
#include <string_view>
#include <optional>
#include <functional>
#include <stdexcept>

struct LanguageScannerError : public std::runtime_error
{
    LanguageScannerError(const std::string& message) : 
        std::runtime_error(message)
    {
    }
};

/**
 * @brief Provides helper function for scanning a std::string_view 
 */
class StringViewScanner
{
private:
    const std::string_view m_source;
    std::size_t m_index;
public:
    bool atEnd() const;

    // returns the scanners current character
    // if it is at the end, it will return std::nullopt 
    std::optional<char> peek() const;

    // advances the scanner forward a character and returns the new
    // current character
    std::optional<char> advance();

    // returns the current character and advances the scanner
    std::optional<char> consume();
    
    // only consumes if the current character matches the one provided
    std::optional<char> consume(char c);

    // only consumes if the current character satisfies the given predicate 
    std::optional<char> consume(std::function<bool(char)> fun);

    // consumes the current character expecting it to be the given character
    // if it does not match a LanguageScannerError is thrown
    char expect(char c);

    // same as above but throws an error if the current character does 
    // satisfy the predicate
    char expect(std::function<bool(char)> fun);

    // repeatedly consumes whitespace until the current charcter is 
    // no longer whitespace

    void dropWhitespace();
    
    StringViewScanner(const std::string_view sv);
};