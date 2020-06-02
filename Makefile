# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/hgfs/UbuntuSharedoc/project/lizi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/UbuntuSharedoc/project/lizi

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/hgfs/UbuntuSharedoc/project/lizi/CMakeFiles /mnt/hgfs/UbuntuSharedoc/project/lizi/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/hgfs/UbuntuSharedoc/project/lizi/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named myclient

# Build rule for target.
myclient: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 myclient
.PHONY : myclient

# fast build rule for target.
myclient/fast:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/build
.PHONY : myclient/fast

#=============================================================================
# Target rules for targets named mytest

# Build rule for target.
mytest: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 mytest
.PHONY : mytest

# fast build rule for target.
mytest/fast:
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/build
.PHONY : mytest/fast

#=============================================================================
# Target rules for targets named lizi_base

# Build rule for target.
lizi_base: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 lizi_base
.PHONY : lizi_base

# fast build rule for target.
lizi_base/fast:
	$(MAKE) -f bases/CMakeFiles/lizi_base.dir/build.make bases/CMakeFiles/lizi_base.dir/build
.PHONY : lizi_base/fast

#=============================================================================
# Target rules for targets named lizi_nets

# Build rule for target.
lizi_nets: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 lizi_nets
.PHONY : lizi_nets

# fast build rule for target.
lizi_nets/fast:
	$(MAKE) -f nets/CMakeFiles/lizi_nets.dir/build.make nets/CMakeFiles/lizi_nets.dir/build
.PHONY : lizi_nets/fast

# target to build an object file
HttpClientPressTest.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/HttpClientPressTest.o
.PHONY : HttpClientPressTest.o

# target to preprocess a source file
HttpClientPressTest.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/HttpClientPressTest.i
.PHONY : HttpClientPressTest.i

# target to generate assembly for a file
HttpClientPressTest.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/HttpClientPressTest.s
.PHONY : HttpClientPressTest.s

# target to build an object file
HttpTest.o:
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/HttpTest.o
.PHONY : HttpTest.o

# target to preprocess a source file
HttpTest.i:
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/HttpTest.i
.PHONY : HttpTest.i

# target to generate assembly for a file
HttpTest.s:
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/HttpTest.s
.PHONY : HttpTest.s

# target to build an object file
nets/Acceptor.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Acceptor.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Acceptor.o
.PHONY : nets/Acceptor.o

# target to preprocess a source file
nets/Acceptor.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Acceptor.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Acceptor.i
.PHONY : nets/Acceptor.i

# target to generate assembly for a file
nets/Acceptor.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Acceptor.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Acceptor.s
.PHONY : nets/Acceptor.s

# target to build an object file
nets/Buffer.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Buffer.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Buffer.o
.PHONY : nets/Buffer.o

# target to preprocess a source file
nets/Buffer.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Buffer.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Buffer.i
.PHONY : nets/Buffer.i

# target to generate assembly for a file
nets/Buffer.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Buffer.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Buffer.s
.PHONY : nets/Buffer.s

# target to build an object file
nets/Channel.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Channel.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Channel.o
.PHONY : nets/Channel.o

# target to preprocess a source file
nets/Channel.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Channel.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Channel.i
.PHONY : nets/Channel.i

# target to generate assembly for a file
nets/Channel.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Channel.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Channel.s
.PHONY : nets/Channel.s

# target to build an object file
nets/Connector.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Connector.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Connector.o
.PHONY : nets/Connector.o

# target to preprocess a source file
nets/Connector.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Connector.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Connector.i
.PHONY : nets/Connector.i

# target to generate assembly for a file
nets/Connector.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Connector.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Connector.s
.PHONY : nets/Connector.s

# target to build an object file
nets/EpollPoller.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/EpollPoller.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/EpollPoller.o
.PHONY : nets/EpollPoller.o

# target to preprocess a source file
nets/EpollPoller.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/EpollPoller.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/EpollPoller.i
.PHONY : nets/EpollPoller.i

# target to generate assembly for a file
nets/EpollPoller.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/EpollPoller.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/EpollPoller.s
.PHONY : nets/EpollPoller.s

# target to build an object file
nets/EventLoop.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/EventLoop.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/EventLoop.o
.PHONY : nets/EventLoop.o

# target to preprocess a source file
nets/EventLoop.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/EventLoop.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/EventLoop.i
.PHONY : nets/EventLoop.i

# target to generate assembly for a file
nets/EventLoop.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/EventLoop.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/EventLoop.s
.PHONY : nets/EventLoop.s

# target to build an object file
nets/InetAddr.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/InetAddr.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/InetAddr.o
.PHONY : nets/InetAddr.o

# target to preprocess a source file
nets/InetAddr.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/InetAddr.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/InetAddr.i
.PHONY : nets/InetAddr.i

# target to generate assembly for a file
nets/InetAddr.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/InetAddr.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/InetAddr.s
.PHONY : nets/InetAddr.s

# target to build an object file
nets/PollPoller.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/PollPoller.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/PollPoller.o
.PHONY : nets/PollPoller.o

# target to preprocess a source file
nets/PollPoller.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/PollPoller.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/PollPoller.i
.PHONY : nets/PollPoller.i

# target to generate assembly for a file
nets/PollPoller.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/PollPoller.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/PollPoller.s
.PHONY : nets/PollPoller.s

# target to build an object file
nets/Socket.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Socket.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Socket.o
.PHONY : nets/Socket.o

# target to preprocess a source file
nets/Socket.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Socket.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Socket.i
.PHONY : nets/Socket.i

# target to generate assembly for a file
nets/Socket.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Socket.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Socket.s
.PHONY : nets/Socket.s

# target to build an object file
nets/SocketOpts.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/SocketOpts.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/SocketOpts.o
.PHONY : nets/SocketOpts.o

# target to preprocess a source file
nets/SocketOpts.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/SocketOpts.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/SocketOpts.i
.PHONY : nets/SocketOpts.i

# target to generate assembly for a file
nets/SocketOpts.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/SocketOpts.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/SocketOpts.s
.PHONY : nets/SocketOpts.s

# target to build an object file
nets/TcpClient.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpClient.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpClient.o
.PHONY : nets/TcpClient.o

# target to preprocess a source file
nets/TcpClient.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpClient.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpClient.i
.PHONY : nets/TcpClient.i

# target to generate assembly for a file
nets/TcpClient.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpClient.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpClient.s
.PHONY : nets/TcpClient.s

# target to build an object file
nets/TcpConnection.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpConnection.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpConnection.o
.PHONY : nets/TcpConnection.o

# target to preprocess a source file
nets/TcpConnection.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpConnection.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpConnection.i
.PHONY : nets/TcpConnection.i

# target to generate assembly for a file
nets/TcpConnection.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpConnection.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpConnection.s
.PHONY : nets/TcpConnection.s

# target to build an object file
nets/TcpServer.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpServer.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpServer.o
.PHONY : nets/TcpServer.o

# target to preprocess a source file
nets/TcpServer.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpServer.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpServer.i
.PHONY : nets/TcpServer.i

# target to generate assembly for a file
nets/TcpServer.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TcpServer.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TcpServer.s
.PHONY : nets/TcpServer.s

# target to build an object file
nets/Timer.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Timer.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Timer.o
.PHONY : nets/Timer.o

# target to preprocess a source file
nets/Timer.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Timer.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Timer.i
.PHONY : nets/Timer.i

# target to generate assembly for a file
nets/Timer.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/Timer.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/Timer.s
.PHONY : nets/Timer.s

# target to build an object file
nets/TimerQueue.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TimerQueue.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TimerQueue.o
.PHONY : nets/TimerQueue.o

# target to preprocess a source file
nets/TimerQueue.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TimerQueue.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TimerQueue.i
.PHONY : nets/TimerQueue.i

# target to generate assembly for a file
nets/TimerQueue.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/TimerQueue.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/TimerQueue.s
.PHONY : nets/TimerQueue.s

# target to build an object file
nets/http/AnalyzeHttp.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/AnalyzeHttp.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/AnalyzeHttp.o
.PHONY : nets/http/AnalyzeHttp.o

# target to preprocess a source file
nets/http/AnalyzeHttp.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/AnalyzeHttp.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/AnalyzeHttp.i
.PHONY : nets/http/AnalyzeHttp.i

# target to generate assembly for a file
nets/http/AnalyzeHttp.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/AnalyzeHttp.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/AnalyzeHttp.s
.PHONY : nets/http/AnalyzeHttp.s

# target to build an object file
nets/http/HttpRequest.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpRequest.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpRequest.o
.PHONY : nets/http/HttpRequest.o

# target to preprocess a source file
nets/http/HttpRequest.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpRequest.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpRequest.i
.PHONY : nets/http/HttpRequest.i

# target to generate assembly for a file
nets/http/HttpRequest.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpRequest.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpRequest.s
.PHONY : nets/http/HttpRequest.s

# target to build an object file
nets/http/HttpRespone.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpRespone.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpRespone.o
.PHONY : nets/http/HttpRespone.o

# target to preprocess a source file
nets/http/HttpRespone.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpRespone.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpRespone.i
.PHONY : nets/http/HttpRespone.i

# target to generate assembly for a file
nets/http/HttpRespone.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpRespone.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpRespone.s
.PHONY : nets/http/HttpRespone.s

# target to build an object file
nets/http/HttpServer.o:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpServer.o
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpServer.o
.PHONY : nets/http/HttpServer.o

# target to preprocess a source file
nets/http/HttpServer.i:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpServer.i
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpServer.i
.PHONY : nets/http/HttpServer.i

# target to generate assembly for a file
nets/http/HttpServer.s:
	$(MAKE) -f CMakeFiles/myclient.dir/build.make CMakeFiles/myclient.dir/nets/http/HttpServer.s
	$(MAKE) -f CMakeFiles/mytest.dir/build.make CMakeFiles/mytest.dir/nets/http/HttpServer.s
.PHONY : nets/http/HttpServer.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... myclient"
	@echo "... mytest"
	@echo "... lizi_base"
	@echo "... lizi_nets"
	@echo "... HttpClientPressTest.o"
	@echo "... HttpClientPressTest.i"
	@echo "... HttpClientPressTest.s"
	@echo "... HttpTest.o"
	@echo "... HttpTest.i"
	@echo "... HttpTest.s"
	@echo "... nets/Acceptor.o"
	@echo "... nets/Acceptor.i"
	@echo "... nets/Acceptor.s"
	@echo "... nets/Buffer.o"
	@echo "... nets/Buffer.i"
	@echo "... nets/Buffer.s"
	@echo "... nets/Channel.o"
	@echo "... nets/Channel.i"
	@echo "... nets/Channel.s"
	@echo "... nets/Connector.o"
	@echo "... nets/Connector.i"
	@echo "... nets/Connector.s"
	@echo "... nets/EpollPoller.o"
	@echo "... nets/EpollPoller.i"
	@echo "... nets/EpollPoller.s"
	@echo "... nets/EventLoop.o"
	@echo "... nets/EventLoop.i"
	@echo "... nets/EventLoop.s"
	@echo "... nets/InetAddr.o"
	@echo "... nets/InetAddr.i"
	@echo "... nets/InetAddr.s"
	@echo "... nets/PollPoller.o"
	@echo "... nets/PollPoller.i"
	@echo "... nets/PollPoller.s"
	@echo "... nets/Socket.o"
	@echo "... nets/Socket.i"
	@echo "... nets/Socket.s"
	@echo "... nets/SocketOpts.o"
	@echo "... nets/SocketOpts.i"
	@echo "... nets/SocketOpts.s"
	@echo "... nets/TcpClient.o"
	@echo "... nets/TcpClient.i"
	@echo "... nets/TcpClient.s"
	@echo "... nets/TcpConnection.o"
	@echo "... nets/TcpConnection.i"
	@echo "... nets/TcpConnection.s"
	@echo "... nets/TcpServer.o"
	@echo "... nets/TcpServer.i"
	@echo "... nets/TcpServer.s"
	@echo "... nets/Timer.o"
	@echo "... nets/Timer.i"
	@echo "... nets/Timer.s"
	@echo "... nets/TimerQueue.o"
	@echo "... nets/TimerQueue.i"
	@echo "... nets/TimerQueue.s"
	@echo "... nets/http/AnalyzeHttp.o"
	@echo "... nets/http/AnalyzeHttp.i"
	@echo "... nets/http/AnalyzeHttp.s"
	@echo "... nets/http/HttpRequest.o"
	@echo "... nets/http/HttpRequest.i"
	@echo "... nets/http/HttpRequest.s"
	@echo "... nets/http/HttpRespone.o"
	@echo "... nets/http/HttpRespone.i"
	@echo "... nets/http/HttpRespone.s"
	@echo "... nets/http/HttpServer.o"
	@echo "... nets/http/HttpServer.i"
	@echo "... nets/http/HttpServer.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
