/* 
 * kMancala - kmancalahistory.h
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
#ifndef KMANCALAHISTORY_H
#define KMANCALAHISTORY_H

#include <QListWidget>
#include <QList>

#include "kmancalahistoryitem.h"

#include "player.h"
#include "board.h"

class kMancalaHistory : public QListWidget {
    Q_OBJECT
public:
	kMancalaHistory(QWidget *parent);
	void init(player **p, board *b);
	void init(QList<kMancalaHistoryItem*>);

	void addState(player **p, board *b, int playerLastTurn, int playerNextTurn, int move);
	kMancalaHistoryItem *getState(int id);
	kMancalaHistoryItem *getLast(void);
	QList<kMancalaHistoryItem*> dump(void);
	void removeState(int id);
};

#endif // kMancalaHistory
