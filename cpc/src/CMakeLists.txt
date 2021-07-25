cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
include(FindPkgConfig)
#set(CMAKE_CXX_FLAGS "-O2")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set (OS_LINUX YES)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set (OS_WINDOWS YES)
else()
    message(STATUS "Unknown OS ${CMAKE_SYSTEM_NAME}.")
endif()

set(coroutine_required chgtest;chg024;chg025;chg026;chg027;chg029;chg037;chg048;chg050)
set(libprocps_required chg033)
set(boost_fs_required chg034)
set(pthread_required chg047;chg061;chg062;chg063;chg064;chg065;chg066)

find_package(Threads REQUIRED)

if(OS_LINUX)
    # 上手く探せない
    pkg_check_modules(PROCPS libprocps)
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
find_package(Boost COMPONENTS filesystem system REQUIRED)
make_execs()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    foreach(boost_fs_targ IN LISTS boost_fs_required)
        target_link_libraries(${boost_fs_targ} PRIVATE Boost::filesystem)
    endforeach()
endif()

foreach(coro_targ IN LISTS coroutine_required)
    target_compile_options(${coro_targ} PRIVATE "$<$<CXX_COMPILER_ID:GNU>:-fcoroutines>")
    target_compile_options(${coro_targ} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/await>")
endforeach()

if(PROCPS_FOUND)
    foreach(procps_targ IN LISTS libprocps_required)
        target_link_libraries(${procps_targ} PRIVATE "$<$<PLATFORM_ID:Linux>:${PROCPS_LIBRARIES}>")
        target_compile_definitions(${procps_targ} PRIVATE USE_PROCPS=1)
    endforeach()
endif()

foreach (pthread_targ IN LISTS pthread_required)
    target_link_libraries(${pthread_targ} Threads::Threads)
endforeach()

#add_executable(chg001 chg001.cxx)
#if(CMAKE_COMPILER_IS_GNUCXX)
#    target_link_libraries(async0 pthread)
#endif()
#target_link_libraries(async0 Threads::Threads)
