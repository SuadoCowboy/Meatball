file(GLOB LIBRARIES_REQUIRED ${CMAKE_SOURCE_DIR}/libs/*)
file(COPY ${LIBRARIES_REQUIRED} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(SpaceInvaders ${PROJECT_SOURCE_DIR}/tests/SpaceInvaders/Game.cpp)
target_include_directories(SpaceInvaders PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(SpaceInvaders Meatball)

add_executable(CpmdStats ${PROJECT_SOURCE_DIR}/tests/CpmdStats/Test.cpp)
target_include_directories(CpmdStats PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(CpmdStats Meatball)