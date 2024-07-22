file(GLOB LIBRARIES_REQUIRED ${CMAKE_SOURCE_DIR}/bin/*)
file(COPY ${LIBRARIES_REQUIRED} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/tests/SpaceInvaders/src/*.cpp)
add_executable(SpaceInvaders ${SOURCES})
target_include_directories(SpaceInvaders PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/tests/SpaceInvaders/src)
target_link_libraries(SpaceInvaders Meatball)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/tests/UIMaker/src/*.cpp)
add_executable(UIMaker ${SOURCES})
target_include_directories(UIMaker PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/tests/UIMaker/src)
target_link_libraries(UIMaker Meatball)