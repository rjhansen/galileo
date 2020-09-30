cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

project(Galileo VERSION 1.0.0 DESCRIPTION "A sightline calculator")

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(SOURCECODE src/main.cpp src/mainwindow.cpp src/about.cpp src/Galileo.qrc src/mainwindow.ui src/about.ui)

set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/rjhansen/Galileo")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Rob Hansen <rob@hansen.engineering>")
set(CPACK_PACKAGE_VENDOR "Hansen Engineering")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_GENERATOR "RPM")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


if (UNIX)
    include(GNUInstallDirs)
    set(CPACK_RPM_USER_BINARY_SPECFILE "project.spec")
    configure_file("project.spec.in" "project.spec" @ONLY IMMEDIATE)
    add_executable(${PROJECT_NAME} ${SOURCECODE})
endif(UNIX)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    add_executable(${PROJECT_NAME} WIN32 ${SOURCECODE})
    configure_file("template.wxs.in" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wxs")
endif(MSVC)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED true
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${PROJECT_VERSION}")

if (UNIX)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif(UNIX)

include(CPack)
