# CMake Builder - kdb Integration with systemd Notification Mechanism
# Copyright (c) 2021 Jaskirat Rajasansir

cmake_minimum_required(VERSION 3.9)

# Global variables
set(APP_NAME        kdb-cpp-common)
set(MAJOR_VERSION   1)
set(FULL_VERSION    ${MAJOR_VERSION}.0.0)

set(APP_DESCRPTION "C++ based common libraries for kdb+")
set(APP_LICENSE "Apache 2.0")
set(APP_URL "https://github.com/jasraj/kdb-cpp-common-lib")
set(APP_VENDOR "Jaskirat Rajasansir")


project(    ${APP_NAME}
            LANGUAGES CXX
            VERSION ${FULL_VERSION})

add_library(    ${PROJECT_NAME} SHARED
                src/Common.cpp
                src/Time.cpp

                src/Base64.cpp
                src/File.cpp
                src/Process.cpp
                src/System.cpp
                )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_target_properties(  ${PROJECT_NAME} PROPERTIES
                        VERSION ${PROJECT_VERSION}
                        SOVERSION ${MAJOR_VERSION})

add_compile_definitions(KXVER=3)

target_include_directories(${PROJECT_NAME} PRIVATE .)

# Local installation with 'cmake --install' and for CPack
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Distribution
set(CPACK_GENERATOR "DEB;RPM;TGZ")
set(CPACK_PACKAGE_NAME lib${APP_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION ${FULL_VERSION})
set(CPACK_PACKAGE_VENDOR ${APP_VENDOR})
set(CPACK_PACKAGE_CONTACT ${APP_VENDOR})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${APP_DESCRPTION})

set(CPACK_RPM_PACKAGE_LICENSE ${APP_LICENSE})
set(CPACK_RPM_PACKAGE_SUMMARY ${APP_DESCRPTION})
set(CPACK_RPM_PACKAGE_DESCRIPTION "${APP_DESCRPTION}\nSee: ${APP_URL}")
set(CPACK_RPM_PACKAGE_AUTOREQPROV YES)

set(CPAK_DEBIAN_PACKAGE_DESCRIPTION "${APP_DESCRPTION}\nSee: ${APP_URL}")

include(CPack)
