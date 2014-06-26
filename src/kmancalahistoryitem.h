/* 
 * kMancala - kmancalahistoryitem.h
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
#ifndef KMANCALAHISTORYITEM_H
#define KMANCALAHISTORYITEM_H

#include <QListWidgetItem>

#include "board.h"
#include "player.h"

class kMancalaHistoryItem : public QListWidgetItem {
public:
	kMancalaHistoryItem(QListWidget *parent, board *b, player **p, int playerLastTurn, int playerNextTurn, int move);

	board *getBoard(void);
	player **getPlayers(void);

	void dump(void);
	int getMove(void);
	int getPlayerOnTurn(void);
	int getPlayerLastTurn(void);

private:
	board *_board;
	player **_players;
	int _playerLastTurn;
	int _playerNextTurn;
	int _move;

};

#endif // kMancalaVIEW_H
