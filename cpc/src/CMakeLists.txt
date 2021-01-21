cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FindPkgConfig)
#set(CMAKE_CXX_FLAGS "-O2")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set (OS_LINUX YES)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set (OS_WINDOWS YES)
else()
    message(STATUS "Unknown OS ${CMAKE_SYSTEM_NAME}.")
endif()

set(coroutine_required chgtest;chg024;chg025;chg026;chg027;chg029)
set(libprocps_required chg033)

find_package(Threads REQUIRED)

if(OS_LINUX)
    # 上手く探せない
    pkg_search_module(PROCPS procps)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

macro(make_execs)
    file(GLOB srcs chg*.cxx)
    foreach(src IN LISTS srcs)
        get_filename_component(exe ${src} NAME_WE) 
        message("SRC: " ${src} ", exe:" ${exe})
        add_executable(${exe} ${src})
    endforeach(src)
endmacro()

macro(lsp_createdb)
    find_program(LSPDB compiledb)
    if(LSPDB)
        exec_program(${LSPDB} ARGS -o ${CMAKE_SOURCE_DIR}/compile_command.json make)
    endif()
endmacro()

#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
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

make_execs()
foreach(coro_targ IN LISTS coroutine_required)
    target_compile_options(${coro_targ} PRIVATE "$<$<CXX_COMPILER_ID:GNU>:-fcoroutines>")
    target_compile_options(${coro_targ} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/await>")
endforeach()

foreach(procps_targ IN LISTS libprocps_required)
    target_link_libraries(${procps_targ} PRIVATE "$<$<PLATFORM_ID:Linux>:-lprocps>")
endforeach()

#add_executable(chg001 chg001.cxx)
#if(CMAKE_COMPILER_IS_GNUCXX)
#    target_link_libraries(async0 pthread)
#endif()
#target_link_libraries(async0 Threads::Threads)
