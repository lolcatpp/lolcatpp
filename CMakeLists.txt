cmake_minimum_required(VERSION 3.26...4.3)
project(lolcat VERSION 2.5.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Makes development easier

# default to Release
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified: defaulting to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

if (BUILD_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
    # Make sure that we enable the Static Runtime /MT
    if (MSVC)
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

include(CheckIncludeFileCXX)
check_include_file_cxx("format" HAS_FORMAT)
if(NOT HAS_FORMAT)
    find_package(fmt REQUIRED)
endif()
find_package(Boost COMPONENTS program_options REQUIRED)

configure_file(
        "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
        "${PROJECT_BINARY_DIR}/version.hpp"
        @ONLY
)
include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Boost::program_options)
if(NOT HAS_FORMAT)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_LIBFMT)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
endif()

# Compilator options & Warnings
if (MSVC)
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
        _CRT_NONSTDC_NO_DEPRECATE
    )
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    if (BUILD_STATIC AND NOT APPLE)
        target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
    endif()
endif ()

install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION bin
        COMPONENT runtime
)

# Debian and Fedora have different guidelines on where the license should end up
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE"
        DESTINATION share/doc/lolcat++
        RENAME copyright
        COMPONENT deb_license
        EXCLUDE_FROM_ALL
)
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE"
        DESTINATION share/licenses/lolcat++
        COMPONENT rpm_license
        EXCLUDE_FROM_ALL
)

set(CPACK_PROJECT_CONFIG_FILE "${PROJECT_SOURCE_DIR}/cmake/CPackProjectConfig.cmake")

set(CPACK_PACKAGE_NAME "lolcat++")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}${DEB_VERSION_SUFFIX}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Concatenate and rainbow-colorize files (C++ port of lolcat)")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/lolcatpp/lolcatpp")
set(CPACK_PACKAGE_CONTACT "Love Billenius <lovebillenius@disroot.org>")

set(CPACK_GENERATOR "DEB")
# Component-based packaging for deb_license / rpm_license picked per-generator.
# All components selected for a generator are merged into one .deb/.rpm.
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)
set(CPACK_DEBIAN_PACKAGE_NAME "lolcat++")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Love Billenius <lovebillenius@disroot.org>")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${CPACK_PACKAGE_HOMEPAGE_URL}")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_CONFLICTS "lolcat")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

set(CPACK_RPM_PACKAGE_NAME "lolcat++")
set(CPACK_RPM_PACKAGE_LICENSE "BSD-3-Clause")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Text")
set(CPACK_RPM_PACKAGE_URL "${CPACK_PACKAGE_HOMEPAGE_URL}")
set(CPACK_RPM_PACKAGE_AUTOREQ ON)
set(CPACK_RPM_PACKAGE_CONFLICTS "lolcat")
set(CPACK_RPM_FILE_NAME RPM-DEFAULT)

include(CPack)
