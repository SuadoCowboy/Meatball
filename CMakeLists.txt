cmake_minimum_required(VERSION 3.10.0)
project(Meatball VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/Interface/*.cpp)
add_library(Meatball SHARED ${SOURCES})

target_include_directories(Meatball PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(Meatball -L${PROJECT_SOURCE_DIR}/libs -lHayBCMD -lraylib)

add_subdirectory(${PROJECT_SOURCE_DIR}/examples)