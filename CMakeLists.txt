cmake_minimum_required(VERSION 3.8)  # cmake的最低版本要求。可通过cmake --version查看目前版本。
project(run)  # 新建一个工程项目，名字任取，也为编译出来的可执行文件名。
set(CMAKE_CXX_STANDARD 11)  # 使用C++11标准库编译。
set(SOURCE_FILES main.cpp)
add_executable(run ${SOURCE_FILES})

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# find_package(OpenCV REQUIRED) # 或者find_package(OpenCV3)
# include_directories(${OpenCV_INCLUDE_DIRS})
# target_link_libraries(run ${OpenCV_LIBS})