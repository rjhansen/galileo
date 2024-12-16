# We set our GUI_SOURCECODE variable to contain every .cpp, .c, and Qt file
# that Galileo depends upon.  Declaring header files is not necessary.

set(GUI_SOURCECODE main.cpp mainwindow.cpp about.cpp
    Galileo.qrc mainwindow.ui about.ui)

# Now we dive into the deep weeds of per-platform behavior, but thanks to
# CMake it's not that bad.  On MacOS, applications need to be defined with
# the MACOSX_BUNDLE property; on Win32, the WIN32 property.  On non-MacOS
# UNIX platforms we can include(GNUInstallDirs) to make the install build
# target throw things in the proper directories according to the GNU
# directory standards.

if (APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${GUI_SOURCECODE})
elseif (UNIX)
    include(GNUInstallDirs)
    add_executable(${PROJECT_NAME} ${GUI_SOURCECODE})
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
elseif (MSVC)
    add_executable(${PROJECT_NAME} WIN32 ${GUI_SOURCECODE})
    # Don't forget: you'll probably need to run windeployqt on the executable
    # to copy over all the DLLs you'll need.
endif ()