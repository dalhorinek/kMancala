/* 
 * kMancala - kmancalarenderer.h
 * Copyright (C) 2011 Dalibor Horinek 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */ 
#ifndef KMANCALARENDERER_H
#define KMANCALARENDERER_H

#include <QPixmap>
#include <QString>
#include <QSize>
#include <QSvgRenderer>

class kMancalaRenderer : public QSvgRenderer
{
  Q_OBJECT
  
public:
    kMancalaRenderer(QObject* parent);
    QPixmap renderPixmap(QString name, const QSize size);

public slots:
    void changeTheme(QString file);
};

#endif // KMANCALARENDERER_H
