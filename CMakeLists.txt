cmake_minimum_required(VERSION 3.16)
project(demo_orm_drogon)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(model MODEL_DIR)

add_executable(demo_orm_drogon
        src/main.cpp
        src/SimpleController.cc
        ${MODEL_DIR}
        )

# Link Drogon
include(cmake/CPM.cmake)
CPMAddPackage(
        NAME drogon
        VERSION 1.7.5
        GITHUB_REPOSITORY drogonframework/drogon
        GIT_TAG v1.7.5
)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon)

# Link Boost
find_package(Boost 1.71 REQUIRED filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})

add_compile_definitions(BOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT) # workaround for https://github.com/boostorg/uuid/issues/68

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
        ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME})
