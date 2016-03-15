QMAKE_CXXFLAGS += -std=c++14

QT += core gui network # multimedia
# LIBS += -L/home/quasilyte/STUDY/qt/super_melee/build-SUPER_MELEE-Desktop-Debug/ -lQtMultimedia
# INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/

LIBS += -lvlc

TARGET = SUPER_MELEE
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/frames/game_frame.cpp \
    src/core/battle_scene.cpp \
    src/frames/lobby_frame.cpp \
    src/actors/vessel.cpp \
    src/core/game_scene.cpp \
    src/net/server.cpp \
    src/net/clients_iter.cpp \
    src/net/messages/players_list.cpp \
    src/audio.cpp \
    src/net/socket.cpp \
    src/equipment/engine.cpp \
    src/sprite/animated_sprites.cpp

INCLUDEPATH += src/
HEADERS += \
    src/actors/vessel.h \
    src/core/graphics_item.h \
    src/core/game_item.h \
    src/net/client.h \
    src/net/player.h \
    src/vessels/dreadnought.h \
    src/weapons/weapon.h \
    src/core/game_scene.h \
    src/exceptions.h \
    src/utils/cyclic_value.h \
    src/utils/polar.h \
    src/utils/bounded_value.h \
    src/frames/lobby_frame.h \
    src/frames/game_frame.h \
    src/ui/messenger.h \
    src/chip/command.h \
    src/chip/control.h \
    src/chip/human_control.h \
    src/chip/random_control.h \
    src/nav_bridge.h \
    src/equipment/engine.h \
    src/equipment/battery.h \
    src/weapons/fusion_blaster.h \
    src/actors/basic_bullet.h \
    src/net/conn/client_connection.h \
    src/net/conn/connection.h \
    src/net/utils.h \
    src/net/config.h \
    src/net/server/chat_controller.h \
    src/utils/constexpr.h \
    src/net/messages/header.h \
    src/net/socket.h \
    src/net/clients_iter.h \
    src/audio.h \
    src/vessels/marauder.h \
    src/weapons/blade_launcher.h \
    src/actors/special_effect.h \
    src/sprite/animated_sprite.h \
    src/net/server/server.h \
    src/utils/defs.h

FORMS += forms/game_frame.ui \
    forms/lobby_frame.ui

