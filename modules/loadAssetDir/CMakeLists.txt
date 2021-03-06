cmake_minimum_required(VERSION 3.7)
project(loadAssetDir)

if(MSVC)
  set(DLLIMPORT "__declspec(dllimport)")
  set(DLLEXPORT "__declspec(dllexport)")
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(PROJECT_SRCS
${PROJECT_SOURCE_DIR}/src/loadAssetDir.cpp
)

set(PROJECT_INCS
${PROJECT_SOURCE_DIR}/include/hvr/loadAssetDir/loadAssetDir.hpp
)

set(PROJECT_XML
#${PROJECT_SOURCE_DIR}/include/hvr/xml/test.xml
${PROJECT_SOURCE_DIR}/include/hvr/xml/input.xml
)

source_group("include" FILES ${PROJECT_INCS})
source_group("src"     FILES ${PROJECT_SRCS})
source_group("xml" FILES ${PROJECT_XML})

find_package( OpenCV REQUIRED )
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package( glog REQUIRED )


add_library(${PROJECT_NAME} SHARED ${PROJECT_SRCS} ${PROJECT_INCS} ${PROJECT_XML} )

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_compile_definitions(${PROJECT_NAME} INTERFACE
                          HVR_RICHARD_LOADASSETDIR_DLL=${DLLIMPORT})

target_compile_definitions(${PROJECT_NAME} PRIVATE
                          HVR_RICHARD_LOADASSETDIR_DLL=${DLLEXPORT})

target_include_directories(${PROJECT_NAME} PUBLIC
                          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                          ${OpenCV_INCLUDE_DIRS}
                          # ${GLEW_INCLUDE_PATH}
                          ${OPENGL_INCLUDE_DIR}
                          # ${TINYXML2_INCLUDE_DIRS}
                          )

target_link_libraries(${PROJECT_NAME} glog::glog glfw GLEW::glew ${OPENGL_LIBRARIES} tinyxml2)

set_property(TARGET ${PROJECT_NAME} PROPERTY
            FOLDER ${CMAKE_PROJECT_NAME}/modules)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES ${PROJECT_SOURCE_DIR}/include/loadAssetDir.hpp
        DESTINATION include)

export(TARGETS ${PROJECT_NAME}
      APPEND FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake)

file(APPEND
    ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    "set(HVR_SAMPLE_LIBRARIES \${HVR_SAMPLE_LIBRARIES} ${PROJECT_NAME})\n")

if(MSVC)
  add_custom_command(TARGET loadAssetDir POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_directory
              ${PROJECT_SOURCE_DIR}/include/hvr/xml/
              ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>)
elseif((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR CMAKE_COMPILER_IS_GNUCC
    OR CMAKE_COMPILER_IS_GNUCXX)

  file(GLOB DENOISE_IMAGES ${PROJECT_SOURCE_DIR}/data/Denoise/*)

  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/denoisedata.cmake
    "file(COPY ${DENOISE_IMAGES} DESTINATION
    ${CMAKE_CURRENT_BINARY_DIR}/data/Denoise
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_WRITE GROUP_READ WORLD_READ)")

  add_custom_command(TARGET DenoiseTest POST_BUILD COMMAND ${CMAKE_COMMAND}
                  -P ${CMAKE_CURRENT_BINARY_DIR}/denoisedata.cmake)
endif()