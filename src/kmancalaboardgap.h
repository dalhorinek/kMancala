/* 
 * kMancala - kmancalaboardgap.h
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
#ifndef KMANCALABOARDGAP_H
#define KMANCALABOARDGAP_H

#include <QGraphicsObject>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QSvgRenderer>
#include <QRectF>

#include "board.h"

class kMancalaBoardGap : public QGraphicsObject
{
  Q_OBJECT
  
public:
	kMancalaBoardGap(QPixmap pGap);
	~kMancalaBoardGap();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

/* events */
	void mousePressEvent ( QGraphicsSceneMouseEvent *event );

signals:
	void clicked();
    
private:
	QPixmap _pixmapGap;
};

#endif // KMANCALABOARDGAP_H
