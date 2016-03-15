#pragma once

#include <vlc/vlc.h>

class MediaFile {
public:
  MediaFile(const char *path);

  ~MediaFile();

private:
  libvlc_media_t *media;

  friend class MediaPlayer;
};

class MediaPlayer {
public:
  static void init();
  static void release();

  MediaPlayer();
  MediaPlayer(const MediaFile& file);
  ~MediaPlayer();

  void play();
  void pause();
  void stop();
  void replay();

private:
  static libvlc_instance_t *vlc;

  libvlc_media_player_t *player;

  friend class MediaFile;
};
