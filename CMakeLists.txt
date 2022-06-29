cmake_minimum_required(VERSION 2.8)
project(sylar)
include (cmake/utils.cmake)
set(CMAKE_VERBOSE_MAKEFILE ON)

if (APPLE)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function  -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -O0 -fPIC  -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
endif()

if(NOT APPLE)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
endif()

#set(CMAKE_MACOSX_RPATH 1)
include_directories(.)
if (APPLE)
set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
include_directories(BEFORE /usr/local/include)
link_directories(BEFORE /usr/local/lib)
endif()

if(NOT APPLE)
include_directories(/apps/sylar/include)
link_directories(/apps/sylar/lib)
link_directories(/apps/sylar/lib64)
endif()

set(LIB_SRC
    sylar/log.cc
    sylar/util.cc
    sylar/config.cc
    sylar/thread.cc
    sylar/util.cc
    )

set(LIBS
    sylar
    pthread
    yaml-cpp
)

add_library(sylar SHARED ${LIB_SRC})

force_redefine_file_macro_for_sources(sylar) #__FILE__

#find_library(YAMLCPP yaml-cpp)
#find_library(PTHREAD pthread)

add_executable(test tests/test.cc)
add_dependencies(test sylar)
force_redefine_file_macro_for_sources(test)
target_link_libraries(test ${LIBS})

#add_executable(test_config tests/test_config.cc)
#add_dependencies(test_config sylar)
#force_redefine_file_macro_for_sources(test_config)
#target_link_libraries(test_config ${LIBS})

sylar_add_executable(test_config "tests/test_config.cc" sylar "${LIBS}")
sylar_add_executable(test_thread "tests/test_thread.cc" sylar "${LIBS}")
sylar_add_executable(test_util "tests/test_util.cc" sylar "${LIBS}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
