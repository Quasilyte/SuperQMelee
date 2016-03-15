# app settings: {
TARGET = SUPER_MELEE
TEMPLATE = app
# }

# compilation settings: {
QMAKE_CXXFLAGS += -std=c++14
INCLUDEPATH += src/
# }

# libs & deps: {
QT += core gui network
LIBS += -lvlc
# }

# sources & headers: {
SOURCES += \
    src/actors/vessel.cpp \
    src/audio.cpp \
    src/core/battle_scene.cpp \
    src/core/game_scene.cpp \
    src/equipment/engine.cpp \
    src/frames/game_frame.cpp \
    src/frames/lobby_frame.cpp \
    src/main.cpp \
    src/net/clients_iter.cpp \
    src/net/messages/players_list.cpp \
    src/net/server.cpp \
    src/net/socket.cpp \
    src/sprite/animated_sprites.cpp

HEADERS += \
src/actors/basic_bullet.h \
    src/actors/special_effect.h \
    src/actors/vessel.h \
    src/audio.h \
    src/chip/command.h \
    src/chip/control.h \
    src/chip/human_control.h \
    src/chip/random_control.h \
    src/core/game_item.h \
    src/core/game_scene.h \
    src/core/graphics_item.h \
    src/equipment/battery.h \
    src/equipment/engine.h \
    src/exceptions.h \
    src/frames/game_frame.h \
    src/frames/lobby_frame.h \
    src/nav_bridge.h \
    src/net/client.h \
    src/net/clients_iter.h \
    src/net/config.h \
    src/net/conn/client_connection.h \
    src/net/conn/connection.h \
    src/net/messages/header.h \
    src/net/player.h \
    src/net/server/chat_controller.h \
    src/net/server/server.h \
    src/net/socket.h \
    src/net/utils.h \
    src/sprite/animated_sprite.h \
    src/ui/messenger.h \
    src/utils/bounded_value.h \
    src/utils/constexpr.h \
    src/utils/cyclic_value.h \
    src/utils/defs.h \
    src/utils/polar.h \
    src/vessels/dreadnought.h \
    src/vessels/marauder.h \
    src/weapons/blade_launcher.h \
    src/weapons/fusion_blaster.h \
    src/weapons/weapon.h
# }

# ui: {
FORMS += forms/game_frame.ui \
    forms/lobby_frame.ui
# }
