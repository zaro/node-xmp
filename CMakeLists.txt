cmake_minimum_required(VERSION 3.1)
include(ExternalProject)

project(xmp VERSION 1.0.0)

set( CMAKE_VERBOSE_MAKEFILE on )

if(NODE_ARCH STREQUAL "x64")
  set(BUILD_64_BIT On) # 64bit
else()
  set(BUILD_64_BIT Off) # 32bit
endif()

get_filename_component(XMPROOT "${CMAKE_SOURCE_DIR}/XMP-Toolkit-SDK-CC201607/" ABSOLUTE)
set(xmp_build_static 'On')
string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_buildtype_lc)

if(BUILD_64_BIT)
  if(NOT APPLE)
    set(cmake_modesubdir _x64)
  else()
    set(cmake_modesubdir _64)
  endif()
else()
  set(cmake_modesubdir "")
endif()

if(APPLE)
  set(XMP_ENV "MAC_ENV")
  set(xmp_toolchain "-DCMAKE_TOOLCHAIN_FILE=${XMPROOT}/build/shared/ToolchainLLVM.cmake")
  set(xmp_cmakedir "xcode/static/${CMAKE_SYSTEM_NAME}${cmake_modesubdir}/${CMAKE_BUILD_TYPE}")
  set(pdf_handler_dir "${XMPROOT}/XMPFilesPlugins/PDF_Handler/macintosh/intel${cmake_modesubdir}")

  set(xmplibdir "${XMPROOT}/public/libraries/macintosh/intel${cmake_modesubdir}/${cmake_buildtype_lc}")
  set(ld_xmplib_core "${xmplibdir}/libXMPCoreStatic.a")
  set(ld_xmplib_files "${xmplibdir}/libXMPFilesStatic.a")

endif()
if(NOT APPLE AND UNIX)
  set(XMP_ENV "UNIX_ENV")
  set(xmp_toolchain "-DCMAKE_TOOLCHAIN_FILE=${XMPROOT}/build/shared/ToolchainGCC.cmake")
  set(xmp_cmakedir "gcc/static/${CMAKE_SYSTEM_NAME}${cmake_modesubdir}/${CMAKE_BUILD_TYPE}")
  set(pdf_handler_dir "${XMPROOT}/XMPFilesPlugins/PDF_Handler/macintosh/intel${cmake_modesubdir}")

  set(xmplibdir "${XMPROOT}/public/libraries/i80386linux${cmake_modesubdir}/${cmake_buildtype_lc}")
  set(ld_xmplib_core "${xmplibdir}/staticXMPCore.ar")
  set(ld_xmplib_files "${xmplibdir}/staticXMPFiles.ar")

endif()
if(WIN32)
  set(XMP_ENV "WIN_ENV")
  set(xmp_toolchain "")
  set(xmp_cmakedir "vc14/static/windows${cmake_modesubdir}")
  set(pdf_handler_dir "${XMPROOT}/XMPFilesPlugins/PDF_Handler/windows/windows${cmake_modesubdir}")

  set(xmplibdir "${XMPROOT}/public/libraries/windows${cmake_modesubdir}/${CMAKE_BUILD_TYPE}")
  set(ld_xmplib_core "${xmplibdir}/XMPCoreStatic.lib")
  set(ld_xmplib_files "${xmplibdir}/XMPFilesStatic.lib")

endif()

ExternalProject_Add(
  XMPSDK

  SOURCE_DIR "${XMPROOT}/build/"
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  CMAKE_ARGS "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}" "-DCMAKE_CL_64=${BUILD_64_BIT}" "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/XMPSDK" "-DXMP_CMAKEFOLDER_NAME=${xmp_cmakedir}" "-DXMP_BUILD_STATIC=${xmp_build_static}"  ${xmp_toolchain}
  INSTALL_COMMAND cmake -E echo "Skipping install step."
)


add_custom_command(TARGET XMPSDK POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${pdf_handler_dir}"
      "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/xfplugins"
)

# Build a shared library named after the project from the files in `src/`
file(GLOB SOURCE_FILES "src/*.cxx" "src/*.h")
if(WIN32)
  file(GLOB SOURCE_FILES_OS "src/windows/*.cxx" "src/windows/*.h")
else()
  file(GLOB SOURCE_FILES_OS "src/unix/*.cxx" "src/unix/*.h")
endif()
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${SOURCE_FILES_OS})
add_dependencies(${PROJECT_NAME} XMPSDK)

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

add_definitions("-D${XMP_ENV}")

include_directories("${XMPROOT}/public/include" "src/")
# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} "${ld_xmplib_core}" "${ld_xmplib_files}")
