#include "frames/game_frame.h"
#include "frames/lobby_frame.h"

#include "net/server.h"
#include "net/client.h"
#include "net/player.h"
#include "net/route.h"
#include "audio.h"
#include <unistd.h>

#include <memory>
#include <QApplication>

#define LOBBY_MODE 1

// http://www.star-control.com/hosted/scsaga/shipspic.htm
// BUGS:
// * disconnect + connect = crash
int main(int argc, char *argv[]) {
  QApplication app{argc, argv};
  app.setStyle("gtk");
  //MediaPlayer::init();

# if LOBBY_MODE == 1
  LobbyFrame frame;
# else
  GameFrame frame;
  //MediaPlayer music{MediaFile{"audio/battle.mod"}};
  //music.play();
# endif
  frame.show();

  return app.exec();
}
