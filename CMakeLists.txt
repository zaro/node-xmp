cmake_minimum_required(VERSION 3.1)

project(nm_test VERSION 1.0.0)

include(ExternalProject)
include(NodeJS.cmake)
nodejs_init()

set(XMPROOT "${CMAKE_SOURCE_DIR}/XMP-Toolkit-SDK-CC201607/")
set(TOOLCHAIN ToolchainLLVM.cmake)
set(compiler xcode)
set(cmake_buildbitdepth On) # 64bit
set(cmake_buildbitdepth On) # 64bit
set(cmake_buildmode Release) # or 'Debug'
set(cmake_build_static 'On')
set(cmake_buildtype static)
set(cmake_modesubdir _64)
set(cmakedir "${compiler}/${cmake_buildtype}/${CMAKE_SYSTEM_NAME}${cmake_modesubdir}/${cmake_buildmode}")
# cmakeconfigdir="../../../../."

ExternalProject_Add(
  XMPSDK

  SOURCE_DIR "${XMPROOT}/build/"
  #SOURCE_DIR "${XMPROOT}/build/"
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  CMAKE_ARGS --debug "-DCMAKE_CL_64=${cmake_buildbitdepth}" "-DCMAKE_BUILD_TYPE=${cmake_buildmode}" "-DXMP_CMAKEFOLDER_NAME=${cmakedir}" "-DXMP_BUILD_STATIC=${cmake_build_static}" "-DCMAKE_TOOLCHAIN_FILE=${XMPROOT}/build/shared/${TOOLCHAIN}"
)

set(XMP_SDK_CXX_FLAGS "-I${XMPROOT}/public/include -DMAC_ENV")

set(CMAKE_CXX_FLAGS_DEBUG "${XMP_SDK_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${XMP_SDK_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${XMP_SDK_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${XMP_SDK_CXX_FLAGS}")
add_nodejs_module(${PROJECT_NAME} node-xmp.cxx node-xmp.h)
