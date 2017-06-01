cmake_minimum_required(VERSION 3.7)
project(CUDASampleTest)

if(MSVC)
  set(DISABLE "__pragma(warning(push,0))")
  set(ENABLE "__pragma(warning(pop))")
endif()

set(CUDASAMPLE_INCS
${PROJECT_SOURCE_DIR}/include/CUDASampleTest.h
)

set(CUDASAMPLE_SRCS
${PROJECT_SOURCE_DIR}/src/CUDASampleTest.cpp
${PROJECT_SOURCE_DIR}/src/main.cpp
)

find_package(CUDA REQUIRED)

cuda_add_executable(CUDASampleTest ${CUDASAMPLE_SRCS} ${CUDASAMPLE_INCS})

target_compile_definitions(${PROJECT_NAME} PUBLIC
                          HVR_WINDOWS_DISABLE_ALL_WARNING=${DISABLE}
                          HVR_WINDOWS_ENABLE_ALL_WARNING=${ENABLE})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

target_link_libraries(${PROJECT_NAME} hvr_cudasample gtest_main)

set_property(TARGET CUDASampleTest PROPERTY
            FOLDER ${CMAKE_PROJECT_NAME}/unit-test)
