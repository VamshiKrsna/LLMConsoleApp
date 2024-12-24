cmake_minimum_required(VERSION 3.15)
project(LLMConsoleApp)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(LLMConsoleApp gemmainference.cpp)
target_link_libraries(LLMConsoleApp PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)