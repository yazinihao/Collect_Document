cmake_minimum_required(VERSION 3.13)
project(CLionProjects CXX C)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include)
include_directories(./googletest/include)

#add_subdirectory(./BinaryTree)
add_subdirectory(./TCPProject)



