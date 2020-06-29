cmake_minimum_required(VERSION 3.9)
project(iroha-nihoheto CXX)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "-O2")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Warning level
if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -pedantic)
endif()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(behaviour behaviour.cxx)
add_executable(fold0 fold0.cxx)
add_executable(async0 async0.cxx)
add_executable(hana0 hana0.cxx)
#if(CMAKE_COMPILER_IS_GNUCXX)
#	target_link_libraries(async0 pthread)
#endif()
target_link_libraries(async0 Threads::Threads)
