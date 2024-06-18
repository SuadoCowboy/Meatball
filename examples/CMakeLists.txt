file(GLOB LIBRARIES_REQUIRED ${CMAKE_SOURCE_DIR}/libs/*)
file(COPY ${LIBRARIES_REQUIRED} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(SpaceInvadersTest ${PROJECT_SOURCE_DIR}/examples/game/SpaceInvadersTest.cpp)
target_include_directories(SpaceInvadersTest PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(SpaceInvadersTest Meatball)

add_executable(MeatDataCompressor ${PROJECT_SOURCE_DIR}/examples/MeatDataCompressor/MeatDataCompressor.cpp)
target_include_directories(MeatDataCompressor PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(MeatDataCompressor Meatball)