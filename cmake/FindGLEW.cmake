# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
#

if(MSVC)
    find_path(GLEW_INCLUDE_PATH GL/glew.h

            $ENV{PROGRAMFILES}/GLEW/include
            $ENV{GLEW_ROOT_DIR}/include
            ${PROJECT_SOURCE_DIR}/src/nvgl/glew/include
        DOC
            "The directory where GL/glew.h resides")


    find_library(GLEW_LIBRARY
        NAMES
            glew GLEW glew32
        PATHS
            $ENV{PROGRAMFILES}/GLEW/lib
            $ENV{GLEW_ROOT_DIR}/lib/Release/x64
            $ENV{GLEW_ROOT_DIR}/bin/Release/x64
            ${PROJECT_SOURCE_DIR}/src/nvgl/glew/bin
            ${PROJECT_SOURCE_DIR}/src/nvgl/glew/lib
        DOC
            "The GLEW library")

else()
    find_path(GLEW_INCLUDE_PATH GL/glew.h

            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        DOC
            "The directory where GL/glew.h resides")

    find_library(GLEW_LIBRARY NO_DEFAULT_PATH
        NAMES
            GLEW glew
        PATHS
            /usr/lib64
            /usr/lib
            /usr/local/lib64
            /usr/local/lib
            /sw/lib
            /opt/local/lib
            $ENV{GLEW_ROOT_DIR}/bin/Release/x64
        DOC
            "The GLEW library")
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIBRARY
                                  GLEW_INCLUDE_PATH)

if(GLEW_INCLUDE_PATH)
    set(GLEW_FOUND 1 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
else()
    set(GLEW_FOUND 0 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
endif()

mark_as_advanced( GLEW_FOUND )
