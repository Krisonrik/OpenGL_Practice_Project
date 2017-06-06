# - Find TINYXML2
# Find the native tinyxml2 headers and libraries.
#
# TINYXML2_INCLUDE_DIRS - where to find tinyxml2.h, etc.
# TINYXML2_LIBRARIES    - List of libraries when using tinyxml2.
# TINYXML2_ROOT_DIR     - The base directory to search for tinyxml2.
#                         This can also be an environment variable.

# Look for root directory on Windows
set(TINYXML2_ROOT_DIR $ENV{TINYXML2_ROOT_DIR}
  CACHE PATH "Root directory to search for tinyxml2")
set(PFX86 "PROGRAMFILES(X86)")
set(PF    "PROGRAMFILES")

# Look for header file.
find_path(TINYXML2_INCLUDE_DIR
  NAMES tinyxml2.h
  PATHS $ENV{${PFX86}}/tinyxml2/include
        $ENV{${PF}}/tinyxml2/include
        ${TINYXML2_ROOT_DIR}/tinyxml2/include
        /usr/local/include
  DOC "TinyXML2 include directory"
  NO_DEFAULT_PATH)

# Look for library.
find_library(TINYXML2_LIBRARY
  NAMES libtinyxml2 tinyxml2
  PATHS $ENV{${PFX86}}/tinyxml2/lib
        $ENV{${PF}}/tinyxml2/lib
        ${TINYXML2_ROOT_DIR}/tinyxml2/include
        /usr/local/lib
  DOC "TinyXML2 library"
  NO_DEFAULT_PATH)

# Find major version (optional)
if(TINYXML2_INCLUDE_DIR)
  foreach(_tinyxml2_version_header tinyxml2.h)
    if(EXISTS "${TINYXML2_INCLUDE_DIR}/${_tinyxml2_version_header}")
      string(CONCAT replacestr "^static[\\t ]const[\\t ]int[\\t ]"
        "TIXML2_MAJOR_VERSION[\\t ]=[\\t ]+([0-9]).*")
      file(STRINGS
        "${TINYXML2_INCLUDE_DIR}/${_tinyxml2_version_header}"
        tinyxml2_version_str REGEX
        "^static[\t ]const[\t ]int[\t ]TIXML2_MAJOR_VERSION[\t ]=[\t ][0-9]")
      string(REGEX REPLACE
        ${replacestr} "\\1"
        TINYXML2_MAJOR_VERSION "${tinyxml2_version_str}")
      unset(tinyxml2_version_str)
      break()
    endif()
  endforeach()
endif()

# Find minor version (optional)
if(TINYXML2_INCLUDE_DIR)
  foreach(_tinyxml2_version_header tinyxml2.h)
    if(EXISTS "${TINYXML2_INCLUDE_DIR}/${_tinyxml2_version_header}")
      string(CONCAT replacestr "^static[\\t ]const[\\t ]int[\\t ]"
        "TIXML2_MINOR_VERSION[\\t ]=[\\t ]+([0-9]).*")
      file(STRINGS
        "${TINYXML2_INCLUDE_DIR}/${_tinyxml2_version_header}"
        tinyxml2_version_str REGEX
        "^static[\t ]const[\t ]int[\t ]TIXML2_MINOR_VERSION[\t ]=[\t ][0-9]")
      string(REGEX REPLACE
        ${replacestr} "\\1"
        TINYXML2_MINOR_VERSION "${tinyxml2_version_str}")
      unset(tinyxml2_version_str)
      break()
    endif()
  endforeach()
endif()

# Merge verion string (optional)
string(CONCAT TINYXML2_VERSION_STRING
  ${TINYXML2_MAJOR_VERSION}
  "."
  ${TINYXML2_MINOR_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TINYXML2 REQUIRED_VARS
                                  TINYXML2_LIBRARY
                                  TINYXML2_INCLUDE_DIR
                                  VERSION_VAR
                                  TINYXML2_VERSION_STRING)

if(TINYXML2_FOUND)
  set(TINYXML2_INCLUDE_DIRS ${TINYXML2_INCLUDE_DIR}
    CACHE PATH "TinyXML2 include directories")

  set(TINYXML2_LIBRARIES ${TINYXML2_LIBRARY}
    CACHE PATH "TinyXML2 libraries")
endif()
