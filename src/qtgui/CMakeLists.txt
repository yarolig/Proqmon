find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(proqmon
    proqmon.cpp
    monitorwindow.h monitorwindow.cpp monitorwindow.ui
    filterwindow.h filterwindow.cpp filterwindow.ui
    aboutwidget.h aboutwidget.cpp aboutwidget.ui
    res.qrc
    )

target_link_libraries(proqmon
  configuration-static
  tracer-static
  storage-static
  display-static
  common-static
  bcc-static
  ${CURSES_LIBRARIES}
  -lpanel
  logging-static
  stdc++fs
  Qt5::Widgets
)

target_include_directories(proqmon
  PUBLIC
  ..
  "${PROJECT_BINARY_DIR}"
  )

