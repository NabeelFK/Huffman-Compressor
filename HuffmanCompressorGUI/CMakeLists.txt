cmake_minimum_required(VERSION 3.19)
project(HuffmanCompressorGUI LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Svg)

qt_standard_project_setup()

qt_add_executable(HuffmanCompressorGUI
    WIN32 MACOSX_BUNDLE

    main.cpp
    mainwindow.cpp
    mainwindow.h


    ../src/frequency.cpp
    ../src/huffman_tree.cpp
    ../src/huffman_codes.cpp
    ../src/encoder.cpp
    ../src/decoder.cpp
    ../src/compressor.cpp
    ../src/utils.cpp
    dropframe.h
    dropframe.cpp
    resources.qrc
    ../../../../Downloads/mainwindow.ui
    mainwindow.ui
)

target_include_directories(HuffmanCompressorGUI
    PRIVATE
        ../include
)

target_link_libraries(HuffmanCompressorGUI
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Svg
)

install(TARGETS HuffmanCompressorGUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET HuffmanCompressorGUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
