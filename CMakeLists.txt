cmake_minimum_required(VERSION 3.20)

project(bee-script)

find_package(hermes 1.0.0
    REQUIRED
    PATHS /home/alagyn/bdd/Hermes/dist/cmake
)

add_hermes_grammar(
    GRAMMAR bee-script.hm
    TARGET beescript
)

add_executable(bees
    src/main.cpp
)

target_include_directories(beescript
    PRIVATE
        inc
)

target_include_directories(bees
    PRIVATE
        inc
)

target_link_libraries(bees
    PRIVATE
        beescript
)