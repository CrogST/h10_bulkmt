cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 1)
endif()

project(h10_bulkmt
    LANGUAGES CXX
    VERSION 0.0.0$ENV{TRAVIS_BUILD_NUMBER})

set(bin_name bulkmt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${bin_name}
    main.cpp
    collect.cpp
    report.cpp
    out/out_base.cpp
    out/write_out.cpp
    out/log_out.cpp
    .travis.yml
    .doxygen.conf
    )

target_include_directories(${bin_name} PUBLIC
    .
    out
    )

set_target_properties(${bin_name} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
    COMPILE_OPTIONS "-pthread"
    LINK_FLAGS "-pthread"
    COMPILE_OPTIONS "-fsanitize=thread"
    LINK_FLAGS "-fsanitize=thread"
    )

target_link_libraries(${bin_name}
  ${CMAKE_THREAD_LIBS_INIT}
  )

#настройка создания пакета
install(TARGETS ${bin_name} RUNTIME DESTINATION bin)
set(CPACK_PACKAGE_NAME ${bin_name})
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT my_mail@some_mail.com)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)
