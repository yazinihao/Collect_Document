link_directories(../googletest/lib)
add_executable(BaseBinary BaseBinary.cpp)
target_link_libraries(BaseBinary gtest)