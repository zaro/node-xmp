cmake_minimum_required(VERSION 3.1)

project(xmp VERSION 1.0.0)

include(ExternalProject)
include(NodeJS.cmake)
nodejs_init()

set( CMAKE_VERBOSE_MAKEFILE on )

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set(BUILD_64_BIT On) # 64bit
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set(BUILD_64_BIT Off) # 32bit
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )


get_filename_component(XMPROOT "${CMAKE_SOURCE_DIR}/XMP-Toolkit-SDK-CC201607/" ABSOLUTE)
set(cmake_buildtype static)
set(cmake_build_static 'On')
#set(cmake_buildmode Release) # or 'Debug'
set(cmake_buildmode Debug) # or 'Debug'
string(TOLOWER "${cmake_buildmode}" cmake_buildmode_lc)

if(BUILD_64_BIT)
  set(cmake_buildbitdepth On) # 64bit
  if(WIN32)
    set(cmake_modesubdir _x64)
  else()
    set(cmake_modesubdir _64)
  endif()
  set(win_arch "Win64")
else()
  set(cmake_buildbitdepth Off) # 64bit
  set(cmake_modesubdir "")
  set(win_arch "")
endif()

if(APPLE)
  set(cmake_generator ${CMAKE_GENERATOR})
  set(XMP_ENV "MAC_ENV")
  set(TOOLCHAIN "-DCMAKE_TOOLCHAIN_FILE=${XMPROOT}/build/shared/ToolchainLLVM.cmake")
  set(compiler xcode)
  set(cmakedir "${compiler}/${cmake_buildtype}/${CMAKE_SYSTEM_NAME}${cmake_modesubdir}/${cmake_buildmode}")
  set(pdf_handler_dir "${XMPROOT}/XMPFilesPlugins/PDF_Handler/macintosh/intel${cmake_modesubdir}")

  set(xmplibdir "${XMPROOT}/public/libraries/macintosh/intel${cmake_modesubdir}/${cmake_buildmode_lc}")
  set(ld_xmplib_core "-lXMPCoreStatic")
  set(ld_xmplib_files "-lXMPFilesStatic")

  set(XMP_OS_SPECIFIC "node-xmp-unix.cxx")

endif()
if(NOT APPLE AND UNIX)
  set(cmake_generator ${CMAKE_GENERATOR})
  set(XMP_ENV "UNIX_ENV")
  set(TOOLCHAIN "-DCMAKE_TOOLCHAIN_FILE=${XMPROOT}/build/shared/ToolchainGCC.cmake")
  set(compiler gcc)
  set(cmakedir "${compiler}/${cmake_buildtype}/${CMAKE_SYSTEM_NAME}${cmake_modesubdir}/${cmake_buildmode}")
  set(pdf_handler_dir "${XMPROOT}/XMPFilesPlugins/PDF_Handler/macintosh/intel${cmake_modesubdir}")

  set(xmplibdir "${XMPROOT}/public/libraries/i80386linux${cmake_modesubdir}/${cmake_buildmode_lc}")
  set(ld_xmplib_core "${xmplibdir}/staticXMPCore.ar")
  set(ld_xmplib_files "${xmplibdir}/staticXMPFiles.ar")

  set(XMP_OS_SPECIFIC "node-xmp-unix.cxx")

endif()
if(WIN32)
  set(cmake_generator "Visual Studio 14 2015 ${win_arch}")
  set(XMP_ENV "WIN_ENV")
  set(TOOLCHAIN "")
  set(compiler vc14)
  set(cmakedir "${compiler}/${cmake_buildtype}/windows${cmake_modesubdir}")
  set(pdf_handler_dir "${XMPROOT}/XMPFilesPlugins/PDF_Handler/windows/windows${cmake_modesubdir}")

  set(xmplibdir "${XMPROOT}/public/libraries/windows${cmake_modesubdir}/${cmake_buildmode}")
  set(ld_xmplib_core "${xmplibdir}/XMPCoreStatic.lib")
  set(ld_xmplib_files "${xmplibdir}/XMPFilesStatic.lib")

  set(XMP_OS_SPECIFIC "node-xmp-win.cxx")

endif()

ExternalProject_Add(
  XMPSDK

  SOURCE_DIR "${XMPROOT}/build/"
  CMAKE_GENERATOR ${cmake_generator}
  CMAKE_ARGS --debug "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/XMPSDK" "-DCMAKE_CL_64=${cmake_buildbitdepth}" "-DCMAKE_BUILD_TYPE=${cmake_buildmode}" "-DXMP_CMAKEFOLDER_NAME=${cmakedir}" "-DXMP_BUILD_STATIC=${cmake_build_static}"  ${TOOLCHAIN}
  INSTALL_COMMAND cmake -E echo "Skipping install step."
)


add_custom_command(TARGET XMPSDK POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${pdf_handler_dir}"
      "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/xfplugins"
)

add_nodejs_module(${PROJECT_NAME} node-xmp.cxx sxmp-meta-wrapper.cxx sxmp-files-wrapper.cxx xmp-constants.cxx node-xmp.h node-xmp-os.h ${XMP_OS_SPECIFIC})
add_dependencies(${PROJECT_NAME} XMPSDK)
add_definitions("-D${XMP_ENV}")
include_directories("${XMPROOT}/public/include")
link_directories(${xmplibdir})
target_link_libraries(${PROJECT_NAME} "${ld_xmplib_core}" "${ld_xmplib_files}")
