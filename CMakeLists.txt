cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 1)
endif()

project(h07_bulk
    LANGUAGES CXX
    VERSION 0.0.0$ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bulk
    main.cpp
    collect.cpp
    collect.h
    class_out.h
    .travis.yml
    )

set_target_properties(bulk PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
    )

#настройка создания пакета
install(TARGETS bulk RUNTIME DESTINATION bin)
set(CPACK_PACKAGE_NAME bulk)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT my_mail@some_mail.com)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)
