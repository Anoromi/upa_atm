cmake_minimum_required(VERSION 3.5)

project(upa_atm VERSION 0.1 LANGUAGES CXX)

#file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/config LOCAL_PKG_CONFIG_PATH)
#
#set(CMAKE_PREFIX_PATH ${LOCAL_PKG_CONFIG_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)

set(TS_FILES upa_atm_uk_UA.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        login.h
        login.cpp
        login.ui
        mainwindow.ui
        ${TS_FILES}
        middleware/credentials.h middleware/credentials.cpp middleware/transfer.cpp middleware/transfer.h middleware/types.h middleware/tariff.cpp middleware/tariff.h middleware/signedConnection.cpp middleware/signedConnection.h middleware/deposit.h
        middleware/exceptions.h backend/bank.h backend/bankProvider.h backend/bankProvider.cpp middleware/unsignedConnection.h middleware/signedConnection.cpp middleware/signedConnection.h middleware/connectionDetails.h backend/user_card.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(upa_atm
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET upa_atm APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    if (ANDROID)
        add_library(upa_atm SHARED
                ${PROJECT_SOURCES}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(upa_atm
                ${PROJECT_SOURCES}
                )
    endif ()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_link_libraries(upa_atm PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(upa_atm PRIVATE Qt${QT_VERSION_MAJOR}::Sql)

set_target_properties(upa_atm PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS upa_atm
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(upa_atm)
endif ()
