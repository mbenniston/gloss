cmake_minimum_required(VERSION 3.15)

# Create a command that invokes gloss with a given langauge file
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Lang.hpp ${CMAKE_CURRENT_BINARY_DIR}/Lang.cpp
    COMMAND gloss --source ${CMAKE_CURRENT_BINARY_DIR}/Lang.cpp --header ${CMAKE_CURRENT_BINARY_DIR}/Lang.hpp --translator_name Translator ${CMAKE_CURRENT_LIST_DIR}/test.lang
    WORKING_DIRECTORY ../
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/test.lang)

add_executable(gloss-example 
    example.cpp

    # Add a depenency to the translator source file
    ${CMAKE_CURRENT_BINARY_DIR}/Lang.cpp
)

target_compile_features(gloss-example PUBLIC cxx_std_17)

# Include the binary directory to access the generated translated source and header
target_include_directories(gloss-example PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(gloss-example PRIVATE gloss_project_options gloss_project_warnings)