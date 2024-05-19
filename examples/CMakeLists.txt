add_executable(ResizeExample2D ${PROJECT_SOURCE_DIR}/examples/ResizeExample2D.cpp)
target_include_directories(ResizeExample2D PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(ResizeExample2D Meatball)

add_executable(SpaceInvadersTest ${PROJECT_SOURCE_DIR}/examples/SpaceInvadersTest.cpp)
target_include_directories(SpaceInvadersTest PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(SpaceInvadersTest Meatball)