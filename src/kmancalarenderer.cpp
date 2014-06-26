/* 
 * kMancala - kmancalarenderer.cpp 
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
#include "kmancalarenderer.h"

#include <kstandarddirs.h>

#include <QPainter>
#include <QSvgRenderer>
#include <QDebug>

kMancalaRenderer::kMancalaRenderer(QObject *parent = NULL) :
  QSvgRenderer(KStandardDirs::locate("appdata", "themes/default.svgz"), parent) {
  
}

QPixmap kMancalaRenderer::renderPixmap(QString name, const QSize size) {
    QPixmap p(size);
    p.fill(Qt::transparent);

    QPainter painter(&p);

    if ( !elementExists(name)) {
      qErrnoWarning("Element doesn't exist");
      return QPixmap();
    }

	qDebug() << "rendering Pixmap" << name;
    render(&painter, name, QRectF(0,0,size.width(), size.height()));
    painter.end();
	qDebug() << "rendering Pixmap done" << name;

    return p;
}

void kMancalaRenderer::changeTheme(QString file) {
	/* To be done */
}
