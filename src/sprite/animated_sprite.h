#pragma once

#include <QVector>
#include <QString>
#include <QImage>
#include <QPixmap>

#include <cassert>
#include <qdebug.h>

class AnimatedPixmap {
public:
  AnimatedPixmap() = default;

  constexpr AnimatedPixmap(const char *imagePath, int frameWidth, int frameHeight):
  imagePath{imagePath}, frameWidth{frameWidth}, frameHeight{frameHeight} {}

  ~AnimatedPixmap() {
    delete frames;
  }

  void init() {
    assert(!initialized());
    qDebug() << "AnimatedSprite: init" << imagePath;

    QImage atlas{imagePath};
    assert(!atlas.isNull());

    int rows = atlas.height() / frameHeight;
    int cols = atlas.width() / frameWidth;

    frames = new QVector<QPixmap>{};
    frames->reserve(rows * cols);

    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        int offsetX = col * frameWidth;
        int offsetY = row * frameHeight;

        auto frameImage = atlas.copy(offsetX, offsetY, frameWidth, frameHeight);
        frames->push_back(QPixmap::fromImage(frameImage));
      }
    }
  }

  bool initialized() const noexcept {
    return nullptr != frames;
  }

  const QPixmap& getFrame(int index) const { return frames->at(index); }
  int getFrameCount() const noexcept { return frames->size(); }

private:
  const char *imagePath;
  int frameWidth;
  int frameHeight;

  QVector<QPixmap> *frames = nullptr;
};

class AnimatedSprite {
public:
  AnimatedSprite(AnimatedPixmap *frames):
  frames{frames} {}

  const QPixmap& nextFrame() {
    if (currentFrame == frames->getFrameCount()) {
      currentFrame = 0;
    }

    return frames->getFrame(currentFrame++);
  }

  int getFrameCount() const noexcept { return frames->getFrameCount(); }

private:
  AnimatedPixmap *frames;
  int currentFrame = 0;
};

class AnimatedPixmaps {
public:
  enum Index {
    MIN,
    FUSION_BLASTER_EXPLOSION,
    MAX
  };

  static AnimatedPixmap* get(Index index) {
    assert(isLoaded(index) && indexIsValid(index));
    return &cache[index];
  }

  static void load(Index index) {
    assert(!isLoaded(index) && indexIsValid(index));
    cache[index].init();
  }

  static void loadAll() {
    cache[0] = AnimatedPixmap{};
    cache[1] = AnimatedPixmap{"sprites/explo.png", 64, 64};
    cache[3] = AnimatedPixmap{};

    for (int index = MIN + 1; index < MAX; ++index) {
      cache[index].init();
    }
  }

private:
  static AnimatedPixmap cache[MAX + 1];

  static bool indexIsValid(Index index) {
    return index > MIN && index < MAX;
  }

  static bool isLoaded(Index index) {
    return cache[index].initialized();
  }
};
