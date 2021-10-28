#include <iostream>

#include <Lang.hpp>

int main()
{
    std::cout << Translator::getText(Translator::Lang::de, 
                                     Translator::Symbol::Translate_me) << std::endl;

    return 0;
}