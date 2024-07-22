cmake_minimum_required(VERSION 3.10.0)
project(Meatball VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/Interface/*.cpp ${PROJECT_SOURCE_DIR}/src/Utils/*.cpp)

add_library(Meatball SHARED ${SOURCES})
set_target_properties(Meatball PROPERTIES
    VERSION ${PROJECT_VERSION}
    POSITION_INDEPENDENT_CODE 1)

target_include_directories(Meatball PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(Meatball -L${PROJECT_SOURCE_DIR}/bin -lHayBCMD -lraylib)

add_subdirectory(${PROJECT_SOURCE_DIR}/tests)