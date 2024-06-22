file(GLOB LIBRARIES_REQUIRED ${CMAKE_SOURCE_DIR}/libs/*)
file(COPY ${LIBRARIES_REQUIRED} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/tests/SpaceInvaders/src/*.cpp)
add_executable(SpaceInvaders ${SOURCES})
target_include_directories(SpaceInvaders PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/tests/SpaceInvaders/src)
target_link_libraries(SpaceInvaders Meatball)