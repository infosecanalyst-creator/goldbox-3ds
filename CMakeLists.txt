cmake_minimum_required(VERSION 3.16)
project(goldbox-3ds CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GOLDBOX_PC "Build the SDL2 desktop host" ON)
option(GOLDBOX_HEADLESS "Build the headless test runner" ON)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/cs/*.cpp
    ${CMAKE_SOURCE_DIR}/src/Classes/*.cpp
    ${CMAKE_SOURCE_DIR}/src/engine/*.cpp
    ${CMAKE_SOURCE_DIR}/src/host/common/*.cpp)

add_library(goldbox_core STATIC ${CORE_SOURCES})
target_include_directories(goldbox_core PUBLIC ${CMAKE_SOURCE_DIR}/src)
if(MSVC)
    target_compile_options(goldbox_core PUBLIC /bigobj /utf-8)
else()
    # -fwrapv: C# integer arithmetic wraps; make signed overflow defined.
    target_compile_options(goldbox_core PUBLIC -fwrapv -Wall -Wno-unused-variable -Wno-unused-but-set-variable)
endif()
find_package(Threads REQUIRED)
target_link_libraries(goldbox_core PUBLIC Threads::Threads)

if(GOLDBOX_HEADLESS)
    add_executable(goldbox_headless src/host/headless/main.cpp)
    target_link_libraries(goldbox_headless PRIVATE goldbox_core)
endif()

if(GOLDBOX_PC)
    find_package(SDL2 REQUIRED)
    add_executable(goldbox src/host/pc/main.cpp)
    if(TARGET SDL2::SDL2)
        target_link_libraries(goldbox PRIVATE goldbox_core SDL2::SDL2)
        if(TARGET SDL2::SDL2main)
            target_link_libraries(goldbox PRIVATE SDL2::SDL2main)
        endif()
    else()
        target_include_directories(goldbox PRIVATE ${SDL2_INCLUDE_DIRS})
        target_link_libraries(goldbox PRIVATE goldbox_core ${SDL2_LIBRARIES})
    endif()
    add_custom_command(TARGET goldbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/sounds $<TARGET_FILE_DIR:goldbox>/sounds)
endif()
