cmake_minimum_required(VERSION 3.15)

project(gloss)

include(cmake/StandardProjectSettings.cmake)

add_library(gloss_project_options INTERFACE)
add_library(gloss_project_warnings INTERFACE)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(gloss_project_warnings)

include(cmake/Sanitizers.cmake)
enable_sanitizers(gloss_project_options)

add_subdirectory(gloss-reader)
add_subdirectory(gloss)
add_subdirectory(gloss-example)