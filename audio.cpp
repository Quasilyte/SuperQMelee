#include "audio.h"

#include <cassert>

libvlc_instance_t *MediaPlayer::vlc = nullptr;

MediaFile::MediaFile(const char *path) {
  media = libvlc_media_new_path(MediaPlayer::vlc, path);
}

MediaFile::~MediaFile() {
  libvlc_media_release(media);
}

void MediaPlayer::init() {
  assert(vlc == nullptr);
  vlc = libvlc_new(0, nullptr);
}

void MediaPlayer::release() {
  if (vlc) {
    libvlc_release(vlc);
  }
}

MediaPlayer::MediaPlayer() {
  player = libvlc_media_player_new(vlc);
}

MediaPlayer::MediaPlayer(const MediaFile& file) {
  player = libvlc_media_player_new_from_media(file.media);
}

MediaPlayer::~MediaPlayer() {
  libvlc_media_player_release(player);
}

void MediaPlayer::play() {
  libvlc_media_player_play(player);
}

void MediaPlayer::pause() {
  libvlc_media_player_pause(player);
}

void MediaPlayer::stop() {
  libvlc_media_player_stop(player);
}

void MediaPlayer::replay() {
  stop();
  play();
}
