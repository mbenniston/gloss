cmake_minimum_required(VERSION 3.15)

add_library(gloss-reader)
target_compile_features(gloss-reader PUBLIC cxx_std_17)

target_sources(gloss-reader 
PRIVATE
    LanguageReader.cpp    
    LanguageFile.cpp
    StringViewScanner.cpp
)

target_include_directories(gloss-reader PUBLIC .)

target_link_libraries(gloss-reader PRIVATE gloss_project_options gloss_project_warnings)