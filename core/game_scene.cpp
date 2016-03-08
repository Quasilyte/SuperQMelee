#include "game_scene.h"
#include "game_item.h"

void GameScene::destroy(GameItem *item) {
  assert(item != nullptr);

  removeItem(item);
  item->onDestroy();
}
