
project("myDemo")

#将某目录所有文件名都存到变量
aux_source_directory(./ allFile)

find_package(Threads)

link_directories(../bases/)

#add_executable(mytest ${allFile})
#target_link_libraries(mytest  lizi_base ${CMAKE_THREAD_LIBS_INIT})

add_library(lizi_nets ${allFile})
target_link_libraries(lizi_nets ${CMAKE_THREAD_LIBS_INIT} lizi_base)


