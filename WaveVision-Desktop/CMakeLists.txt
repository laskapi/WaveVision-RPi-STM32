cmake_minimum_required(VERSION 3.19)
project(WaveVision-Desktop LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets SerialPort)

qt_standard_project_setup()

qt_add_executable(WaveVision-Desktop
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    serialworker.h serialworker.cpp
    protocol.h
    waveformwidget.h waveformwidget.cpp
)
target_include_directories(WaveVision-Desktop PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(WaveVision-Desktop
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

install(TARGETS WaveVision-Desktop
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WaveVision-Desktop
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
