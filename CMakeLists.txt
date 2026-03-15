cmake_minimum_required(VERSION 3.19)
project(CppCraft_front LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

qt_add_executable(CppCraft_front
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/loginpage.cpp
    src/taskpage.cpp
    ui/mainwindow.ui
    ui/loginpage.ui
    ui/taskpage.ui
    include/loginpage.h
    include/mainwindow.h
    include/task.h
    include/taskpage.h

)

target_link_libraries(CppCraft_front
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS CppCraft_front
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CppCraft_front
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
