cmake_minimum_required(VERSION 3.15)

add_executable(gloss)
target_compile_features(gloss PUBLIC cxx_std_17)

target_sources(gloss 
PRIVATE
    main.cpp
    Generator.cpp
)

target_link_libraries(gloss PRIVATE gloss-reader gloss_project_options gloss_project_warnings)