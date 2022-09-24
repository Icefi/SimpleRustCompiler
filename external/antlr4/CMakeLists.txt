include(FetchContent)

FetchContent_Declare(
    antlr4
    GIT_REPOSITORY https://github.com/adeharo9/antlr4-cpp-runtime.git
    GIT_TAG 4.9.1
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
)

FetchContent_MakeAvailable(antlr4)
