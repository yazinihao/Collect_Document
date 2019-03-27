#add_executable(TCPServer  unp.h TCPServer.cpp)

add_executable(TCPClient unp.h TCPClient.cpp)
target_link_libraries(TCPClient
        -lpthread)
