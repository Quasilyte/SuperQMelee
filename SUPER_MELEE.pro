QMAKE_CXXFLAGS += -std=c++14

QT += core gui network # multimedia
# LIBS += -L/home/quasilyte/STUDY/qt/super_melee/build-SUPER_MELEE-Desktop-Debug/ -lQtMultimedia
# INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/

TARGET = SUPER_MELEE
TEMPLATE = app

SOURCES += \
  main.cpp \
  frames/game_frame.cpp \
    core/vessel.cpp \
    core/battle_scene.cpp \
    frames/lobby_frame.cpp

HEADERS += \
    actors/vessel.h \
    actors/bullet.h \
    core/graphics_item.h \
    core/game_item.h \
    net/server.h \
    net/client.h \
    net/route.h \
    net/player.h \
    vessels/dreadnought.h \
    chip/dicrect_control.h \
    weapons/weapon.h \
    core/game_scene.h \
    exceptions.h \
    utils/cyclic_value.h \
    utils/polar.h \
    utils/bounded_value.h \
    frames/lobby_frame.h \
    frames/game_frame.h \
    ui/messenger.h \
    chip/command.h \
    chip/control.h \
    chip/human_control.h \
    chip/random_control.h \
    nav_bridge.h \
    equipment/engine.h \
    equipment/battery.h

FORMS += frames/forms/game_frame.ui \
    frames/forms/lobby_frame.ui

