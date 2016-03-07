#include "frames/game_frame.h"
#include "frames/lobby_frame.h"

#include "net/server.h"
#include "net/client.h"
#include "net/player.h"
#include "net/route.h"

#include <memory>
#include <QApplication>

#define LOBBY_MODE 0

// http://www.star-control.com/hosted/scsaga/shipspic.htm

int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  app.setStyle("gtk");

# if LOBBY_MODE == 1
  LobbyFrame frame;
# else
  GameFrame frame;
# endif
  frame.show();

  // Client iskander{std::unique_ptr<Player>{new Player{"Iskander"}}};
  // iskander.joinServer(Route{});

  return app.exec();
}
