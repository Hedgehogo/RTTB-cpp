cmake_minimum_required(VERSION 3.24)
project(RTTB_cpp)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

option(RTTB_ENABLE_INSTALL "Enable install rule" ON)

set(ABSL_PROPAGATE_CXX_STD ON)
set(ABSL_ENABLE_INSTALL ${RTTB_ENABLE_INSTALL})

FetchContent_Declare(
        option_result_cpp
        GIT_REPOSITORY https://github.com/Hedgehogo/option-result-cpp.git
        GIT_TAG main
        FIND_PACKAGE_ARGS NAMES option_result_cpp
)

FetchContent_Declare(
        abseil
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp
        GIT_TAG lts_2023_01_25
        FIND_PACKAGE_ARGS NAMES absl
)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.13.0
        FIND_PACKAGE_ARGS NAMES GTest
)

FetchContent_Declare(
        type_name_cpp
        GIT_REPOSITORY https://github.com/Hedgehogo/type-name-cpp
        GIT_TAG master
        FIND_PACKAGE_ARGS NAMES type_name_cpp
)

FetchContent_MakeAvailable(option_result_cpp type_name_cpp googletest)

file(GLOB_RECURSE src "lib/*.hpp" "lib/*.cpp")

add_library(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} PUBLIC option_result_cpp type_name_cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:include>
        )

file(GLOB_RECURSE test_src "test/*.hpp" "test/*.cpp")

add_executable(${PROJECT_NAME}_test ${test_src})

target_link_libraries(${PROJECT_NAME}_test PRIVATE gtest gtest_main ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lib)

target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/test)

if (RTTB_ENABLE_INSTALL)
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/RTTB DESTINATION ${CMAKE_INSTALL_PREFIX}/include PATTERN "*.hpp")

    install(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}
            LIBRARY DESTINATION lib
            PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
            )

    install(EXPORT ${PROJECT_NAME}
            FILE ${PROJECT_NAME}Config.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION lib/cmake/${PROJECT_NAME}
            )
endif ()