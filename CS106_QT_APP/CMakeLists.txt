cmake_minimum_required(VERSION 3.5)

project(CS106_QT_APP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(

    UI_files/
    H_files/
)

set(PROJECT_SOURCES
        main.cpp
        loginwindow.cpp        
        mainwindow.cpp        
        filemanagement.cpp
        userdata.cpp
        newuser.cpp        
        edituser.cpp        
        deleteusers.cpp        
        bookdata.cpp        
        newbook.cpp        
        editbooks.cpp               
        deletebooks.cpp                
        membermenu.cpp
        H_files/mainwindow.h
        H_files/loginwindow.h
        H_files/newuser.h
        H_files/edituser.h
        H_files/deleteusers.h
        H_files/editbooks.h
        H_files/newbook.h
        H_files/bookdata.h
        H_files/deletebooks.h
        H_files/membermenu.h
        H_files/userdata.h
        H_files/filemanagement.h
        UI_files/membermenu.ui
        UI_files/loginwindow.ui
        UI_files/deletebooks.ui
        UI_files/editbooks.ui
        UI_files/newbook.ui
        UI_files/bookdata.ui
        UI_files/deleteusers.ui
        UI_files/edituser.ui
        UI_files/newuser.ui
        UI_files/userdata.ui
        UI_files/mainwindow.ui
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CS106_QT_APP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CS106_QT_APP APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CS106_QT_APP SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CS106_QT_APP
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CS106_QT_APP PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(CS106_QT_APP PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS CS106_QT_APP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CS106_QT_APP)
endif()
