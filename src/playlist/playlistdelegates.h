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

#ifndef PLAYLISTDELEGATES_H
#define PLAYLISTDELEGATES_H

#include "config.h"

#include <QCompleter>
#include <QPixmapCache>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QTreeView>

#include "playlist.h"
#include "collection/collection.h"
#include "widgets/ratingwidget.h"

class Player;

class QueuedItemDelegate : public QStyledItemDelegate {
public:
  QueuedItemDelegate(QObject *parent, int indicator_column = Playlist::Column_Title);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void DrawBox(QPainter *painter, const QRect &line_rect, const QFont &font, const QString &text, int width = -1) const;

  int queue_indicator_size(const QModelIndex &index) const;

private:
  static const int kQueueBoxBorder;
  static const int kQueueBoxCornerRadius;
  static const int kQueueBoxLength;
  static const QRgb kQueueBoxGradientColor1;
  static const QRgb kQueueBoxGradientColor2;
  static const int kQueueOpacitySteps;
  static const float kQueueOpacityLowerBound;

  int indicator_column_;
};

class PlaylistDelegateBase : public QueuedItemDelegate {
  Q_OBJECT
 public:
  PlaylistDelegateBase(QObject *parent, const QString &suffix = QString());
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  QString displayText(const QVariant &value, const QLocale &locale) const;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

  QStyleOptionViewItemV4 Adjusted(const QStyleOptionViewItem &option, const QModelIndex &index) const;

  static const int kMinHeight;

 public slots:
  bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

 protected:
  QTreeView *view_;
  QString suffix_;
};

class LengthItemDelegate : public PlaylistDelegateBase {
 public:
  LengthItemDelegate(QObject *parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant &value, const QLocale &locale) const;
};

class SizeItemDelegate : public PlaylistDelegateBase {
 public:
  SizeItemDelegate(QObject *parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant &value, const QLocale &locale) const;
};

class DateItemDelegate : public PlaylistDelegateBase {
 public:
  DateItemDelegate(QObject *parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant &value, const QLocale &locale) const;
};

class LastPlayedItemDelegate : public PlaylistDelegateBase {
public:
  LastPlayedItemDelegate(QObject *parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant &value, const QLocale &locale) const;
};

class FileTypeItemDelegate : public PlaylistDelegateBase {
 public:
  FileTypeItemDelegate(QObject *parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant &value, const QLocale &locale) const;
};

class SamplerateBitdepthItemDelegate : public PlaylistDelegateBase {
 public:
  SamplerateBitdepthItemDelegate(QObject *parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant &value, const QLocale &locale) const;
};

class TextItemDelegate : public PlaylistDelegateBase {
 public:
  TextItemDelegate(QObject *parent) : PlaylistDelegateBase(parent) {}
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class TagCompletionModel : public QStringListModel {
public:
  TagCompletionModel(CollectionBackend *backend, Playlist::Column column);

private:
  static QString database_column(Playlist::Column column);
};

class TagCompleter : public QCompleter {
  Q_OBJECT

public:
  TagCompleter(CollectionBackend *backend, Playlist::Column column, QLineEdit *editor);

 private slots:
  void ModelReady(QFuture<TagCompletionModel*> future);

private:
  QLineEdit *editor_;
};

class TagCompletionItemDelegate : public PlaylistDelegateBase {
 public:
  TagCompletionItemDelegate(QObject *parent, CollectionBackend *backend, Playlist::Column column) : PlaylistDelegateBase(parent), backend_(backend), column_(column) {};

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
      const QModelIndex &index) const;

 private:
  CollectionBackend *backend_;
  Playlist::Column column_;
};

class NativeSeparatorsDelegate : public PlaylistDelegateBase {
 public:
  NativeSeparatorsDelegate(QObject *parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant &value, const QLocale &locale) const;
};

class SongSourceDelegate : public PlaylistDelegateBase {
 public:
  SongSourceDelegate(QObject *parent, Player *player);
  QString displayText(const QVariant &value, const QLocale &locale) const;
  void paint(QPainter *paint, const QStyleOptionViewItem &option, const QModelIndex &index) const;

 private:
  QPixmap LookupPixmap(const QUrl &url, const QSize &size) const;

  Player *player_;
  mutable QPixmapCache cache_;
};

#endif // PLAYLISTDELEGATES_H
