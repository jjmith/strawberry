/*
 * Strawberry Music Player
 * This file was part of Clementine.
 * Copyright 2010, David Sansome <me@davidsansome.com>
 *
 * Strawberry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Strawberry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Strawberry.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef COLLECTION_H
#define COLLECTION_H

#include "config.h"

#include <QHash>
#include <QObject>
#include <QUrl>

#include "core/song.h"

class Application;
class Database;
class CollectionBackend;
class CollectionModel;
class CollectionWatcher;
class TaskManager;
class Thread;

class Collection : public QObject {
  Q_OBJECT

 public:
  Collection(Application* app, QObject* parent);
  ~Collection();

  static const char *kSongsTable;
  static const char *kDirsTable;
  static const char *kSubdirsTable;
  static const char *kFtsTable;

  void Init();

  CollectionBackend *backend() const { return backend_; }
  CollectionModel *model() const { return model_; }

  QString full_rescan_reason(int schema_version) const { return full_rescan_revisions_.value(schema_version, QString()); }

  int Total_Albums = 0;
  int total_songs_ = 0;
  int Total_Artists = 0;

 public slots:
  void ReloadSettings();

  void PauseWatcher();
  void ResumeWatcher();

  void FullScan();

 private slots:
  void IncrementalScan();

  void CurrentSongChanged(const Song &song);
  void Stopped();

 private:
  SongList FilterCurrentWMASong(SongList songs, Song* queued);

 private:
  Application *app_;
  CollectionBackend *backend_;
  CollectionModel *model_;

  CollectionWatcher *watcher_;
  Thread *watcher_thread_;

  // Hack: Gstreamer doesn't cope well with WMA files being rewritten while
  // being played, so we delay statistics and rating changes until the current
  // song has finished playing.
  QUrl current_wma_song_url_;

  // DB schema versions which should trigger a full collection rescan (each of
  // those with a short reason why).
  QHash<int, QString> full_rescan_revisions_;
};

#endif
