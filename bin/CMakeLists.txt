add_executable(MeatDataCompressor ${PROJECT_SOURCE_DIR}/bin/MeatDataCompressor/MeatDataCompressor.cpp)
target_include_directories(MeatDataCompressor PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(MeatDataCompressor Meatball)