
project("myDemo")

#将某目录所有文件名都存到变量
aux_source_directory(./ allFile)

find_package(Threads)

link_directories(../../bases/)
link_directories(../nets/)

#add_executable(mytest ${allFile})
#target_link_libraries(mytest  lizi_base ${CMAKE_THREAD_LIBS_INIT})

add_library(lizi_http ${allFile})
target_link_libraries(lizi_http ${CMAKE_THREAD_LIBS_INIT} lizi_base lizi_net)


