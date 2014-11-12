
cmake_minimum_required(VERSION 2.8)

# Make directories for the binaries and libraries we create (if they don't exist)
if( CMAKE_BUILD_TYPE STREQUAL Debug )
    set( BUILD_TYPE debug )
    add_definitions( -DVERBOSE ) # Same as having "#define VERBOSE" in all source files
    if(WIN32)
       set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEBUG} /EHsc" ) #/showIncludes
       set( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" )
    else()
       # Compile with debug info, and compile/link with code coverage info
       set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb" )#-fprofile-arcs -ftest-coverage --coverage -p
       set( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11" )
       set( CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage -coverage" )
    endif()
endif()
if( CMAKE_BUILD_TYPE STREQUAL Release )
    set( BUILD_TYPE release )
    add_definitions( -UVERBOSE )
    if(WIN32)
    else()
       set( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11" )
    endif()
endif()

# Create a folder for executables and libraries, and tell CMake to put them there
if( DEFINED INSTALL_DIR )
   message( "   Using custom installation directory: ${INSTALL_DIR}/${BUILD_TYPE}" )
   set( INSTALL_DIR         "${INSTALL_DIR}/${BUILD_TYPE}" )
else()
   message( "   Using default installation directory: ${CMAKE_SOURCE_DIR}/${BUILD_TYPE}" )
   set( INSTALL_DIR         "${CMAKE_SOURCE_DIR}/${BUILD_TYPE}" )
endif()
file( MAKE_DIRECTORY        "${INSTALL_DIR}" )
file( MAKE_DIRECTORY        "${INSTALL_DIR}/bin" )
file( MAKE_DIRECTORY        "${INSTALL_DIR}/lib" )
file( MAKE_DIRECTORY        "${INSTALL_DIR}/include" )
set( EXECUTABLE_OUTPUT_PATH "${INSTALL_DIR}/bin" )
set( LIBRARY_OUTPUT_PATH    "${INSTALL_DIR}/lib" )
set( HEADER_OUTPUT_PATH     "${INSTALL_DIR}/include" )
link_directories(${LIBRARY_OUTPUT_PATH})
include_directories(${HEADER_OUTPUT_PATH})

set(CMAKE_PROGRAM_PATH ${EXECUTABLE_OUTPUT_PATH} ${CMAKE_PROGRAM_PATH})
set(CMAKE_INCLUDE_PATH ${HEADER_OUTPUT_PATH} ${CMAKE_INCLUDE_PATH})
set(CMAKE_LIBRARY_PATH ${LIBRARY_OUTPUT_PATH} ${INSTALL_DIR}/lib64 ${CMAKE_LIBRARY_PATH})

# Make CMake add each directory containing a CMakeLists.txt file to source search path
set( CMAKE_INCLUDE_CURRENT_DIR ON )

add_subdirectory( src )