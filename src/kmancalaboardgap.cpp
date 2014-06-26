/* 
 * kMancala - kmancalaboardgap.cpp 
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
#include <QPixmap>
#include <QGraphicsObject>
#include <QSvgRenderer>
#include <QPainter>

#include "kmancalaboardgap.h"

kMancalaBoardGap::kMancalaBoardGap(QPixmap pGap) : QGraphicsObject() {
	_pixmapGap = pGap;
	setAcceptedMouseButtons(Qt::LeftButton);
}

kMancalaBoardGap::~kMancalaBoardGap() {
}

QRectF kMancalaBoardGap::boundingRect() const {
  return QRectF(0, 0, _pixmapGap.width(), _pixmapGap.height());
}

void kMancalaBoardGap::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
			     QWidget* widget) {

    //qDebug("kMancalaBoardGap::paint(): Painting gap");
    //painter->fillRect(0,0,pixmapGap.width(), pixmapGap.height(), Qt::transparent);
    painter->drawPixmap(0,0,_pixmapGap);
}

void kMancalaBoardGap::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
  qDebug("mouse pressed");
  emit clicked();
}
