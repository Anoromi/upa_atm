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

        middleware/credentials.h middleware/credentials.cpp
        middleware/transfer.cpp middleware/transfer.h
        middleware/types.h middleware/tariff.cpp
        middleware/tariff.h middleware/signedConnection.cpp middleware/signedConnection.h middleware/deposit.h
        middleware/exceptions.h
        backend/bank.h backend/bank.cpp
        backend/bankProvider.h backend/bankProvider.cpp
        middleware/unsignedConnection.h middleware/signedConnection.cpp
        middleware/signedConnection.h middleware/connectionDetails.h
        backend/database/db_card.h backend/database/db_category.h backend/database/db_parent_relation.h backend/database/db_transaction.h
        backend/database/DatabaseException.h
        backend/database/db_util.h backend/database/db_util.cpp
        backend/database/SqlQuery.h
        backend/database/db_card.cpp
        backend/database/db_category.cpp
        backend/database/db_holder.cpp
        backend/database/db_parent_relation.cpp
        backend/database/db_transaction.cpp

        transaction_screen.h transaction_screen.cpp transaction_screen.ui middleware/converters.cpp middleware/converters.h frontend/error_message.cpp frontend/error_message.h
        transaction_details.h transaction_details.cpp transaction_details.ui
        success_screen.h success_screen.cpp success_screen.ui
        navigationwindow.h navigationwindow.cpp navigationwindow.ui
        actionsscreen.h actionsscreen.cpp actionsscreen.ui
        frontend/depositscreen.h frontend/depositscreen.cpp frontend/depositscreen.ui
        frontend/withdrawalscreen.h frontend/withdrawalscreen.cpp frontend/withdrawalscreen.ui
        frontend/put_money.h frontend/put_money.cpp frontend/put_money.ui
        withdrawmoney.h withdrawmoney.cpp withdrawmoney.ui

    )


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
        add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp)
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
