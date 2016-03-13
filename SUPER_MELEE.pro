QMAKE_CXXFLAGS += -std=c++14

QT += core gui network # multimedia
# LIBS += -L/home/quasilyte/STUDY/qt/super_melee/build-SUPER_MELEE-Desktop-Debug/ -lQtMultimedia
# INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/

LIBS += -lvlc

TARGET = SUPER_MELEE
TEMPLATE = app

SOURCES += \
    main.cpp \
    frames/game_frame.cpp \
    core/battle_scene.cpp \
    frames/lobby_frame.cpp \
    actors/vessel.cpp \
    core/game_scene.cpp \
    net/server.cpp \
    net/clients_iter.cpp \
    net/messages/players_list.cpp \
    audio.cpp \
    net/socket.cpp \
    equipment/engine.cpp \
    sprite/animated_sprites.cpp

HEADERS += \
    actors/vessel.h \
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
    equipment/battery.h \
    weapons/fusion_blaster.h \
    actors/basic_bullet.h \
    net/conn/client_connection.h \
    net/conn/connection.h \
    net/utils.h \
    net/config.h \
    utils/constexpr.h \
    net/messages/header.h \
    net/socket.h \
    net/clients_iter.h \
    audio.h \
    vessels/marauder.h \
    weapons/blade_launcher.h \
    actors/special_effect.h \
    sprite/animated_sprite.h

FORMS += frames/forms/game_frame.ui \
    frames/forms/lobby_frame.ui

