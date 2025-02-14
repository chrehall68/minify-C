cmake_minimum_required(VERSION 3.22.1)
project(GolfC VERSION 1.2.0)

# configuration
set(LLVMVersion "17" CACHE STRING "Which LLVM and Clang version to use")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fno-rtti")

find_package(LLVM ${LLVMVersion} REQUIRED)
find_package(Clang ${LLVMVersion} REQUIRED)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(include)

add_definitions(${LLVM_DEFINITIONS})
add_definitions(${CLANG_DEFINITIONS})

include(AddLLVM)
include(/usr/lib/llvm-${LLVMVersion}/lib/cmake/clang/AddClang.cmake)  # not sure why this needed the explicit path...

macro (add_tooling_executable name)
  add_clang_executable(${name} ${ARGN})
  target_link_libraries(
    ${name}  
    PRIVATE
    clangAST
    clangBasic
    clangFrontend
    clangSerialization
    clangTooling
  )
  install(
    TARGETS
    ${name}
    RUNTIME
    DESTINATION
    bin
  )
endmacro(add_tooling_executable)

# debug executable, for development
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_tooling_executable(explorer src/explorer.cpp)
  add_tooling_executable(ciexplorer src/ciexplorer.cpp)
endif()

# actual applications
add_tooling_executable(
  minifier
  # MAIN FILE
  src/main.cpp
  # FORMAT
  src/format/minifyFormat.cpp
  src/format/macroFormat.cpp

  # ACTIONS
  src/actions/minifyAction.cpp
  src/actions/PPSymbolsAction.cpp
  src/actions/expandMacroAction.cpp

  # UTILS
  src/util/symbols.cpp
)

# package
set(LLVMDEP "libllvm${LLVMVersion}")
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "golfC")
set(CPACK_PACKAGE_VENDOR "chrehall68")
set(CPACK_PACK_DEBIAN_ARCHITECTURE ${CMAKE_HOST_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_DEPENDS ${LLVMDEP})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "chrehall68")
include(CPack)