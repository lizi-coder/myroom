

add_subdirectory(bases)
add_subdirectory(nets)

set(Target_File1 "HttpTest.cpp")
set(Target_File2 "HttpClientPressTest.cpp")


set(CMAKE_BUILD_TYPE "Debug")
aux_source_directory(./ TEST_FILE)
aux_source_directory(./nets/ NETS_FILE)
aux_source_directory(./nets/http HTTP_FILE)

include_directories(./nets/)
include_directories(./nets/http/)
add_definitions(-std=c++11)
find_package(Threads)
add_executable(myclient   ${Target_File2} ${NETS_FILE} ${HTTP_FILE})
add_executable(mytest ${Target_File1}  ${NETS_FILE} ${HTTP_FILE} )
target_link_libraries(mytest lizi_base ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(myclient lizi_base ${CMAKE_THREAD_LIBS_INIT})

