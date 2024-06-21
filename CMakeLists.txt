cmake_minimum_required(VERSION 3.20)

project(bee-script)

find_package(hermes 1.0.0
    REQUIRED
    PATHS /home/alagyn/bdd/Hermes/dist/cmake
)

add_hermes_grammar(
    GRAMMAR grammar/bee-script.hm
    GRAMMAR_DIR grammar
    TARGET beescript
    DEBUG
    # DESC_FILE temp.txt
    STRICT
)

add_executable(bees
    src/dag-node.cpp
    src/errors.cpp
    src/main.cpp
    src/symbol.cpp
    src/symbol-table.cpp
    src/type-check.cpp
    
    src/syntax-nodes/decl-node.cpp
    src/syntax-nodes/expr-node.cpp
    src/syntax-nodes/stmt-node.cpp
    src/syntax-nodes/syntax-node.cpp
    src/syntax-nodes/type-node.cpp
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