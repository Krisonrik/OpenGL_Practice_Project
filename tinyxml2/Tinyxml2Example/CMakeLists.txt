cmake_minimum_required(VERSION 3.7)
project(Tinyxml2Example)

if(MSVC)
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/Tinyxml2Example.cpp
)

source_group("src" FILES ${PROJECT_SRCS})

find_package( glog REQUIRED )
find_package( TINYXML2 REQUIRED )

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${TINYXML2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
                      glog::glog
                      ${TINYXML2_LIBRARY})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${CMAKE_PROJECT_NAME}/apps)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        DESTINATION bin)
