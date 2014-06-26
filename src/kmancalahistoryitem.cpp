/* 
 * kMancala - kmancalahistoryitem.cpp 
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
#include <KLocale>

#include <QListWidgetItem>
#include <QDebug>

#include "kmancalahistoryitem.h"
#include "board.h"
#include "player.h"

kMancalaHistoryItem::kMancalaHistoryItem(QListWidget *parent = 0, board *b = 0, player **p = 0, int playerLastTurn = 0, int playerNextTurn = 0, int move = -2)
  : QListWidgetItem(parent, 0) {
	if ( move == -1 ) {
		setText(i18n("Game Start"));
	} else {
		setText(i18n("%1 moved from %2").arg(p[playerLastTurn]->getName()).arg(QString((char)'a'+move)));
	}

	_move = move;
	_board = b;
	_players = p; 
	_playerLastTurn = playerLastTurn;
	_playerNextTurn = playerNextTurn;
	
}

void kMancalaHistoryItem::dump(void) {
	qDebug("History Item: ");
	qDebug() << "Player 1: " << _players[0]->getName() << ", " << _players[0]->getType();
	qDebug() << "Player 1: " << _players[1]->getName() << ", " << _players[1]->getType();
	qDebug("Player 1 at %p, Player 2 at %p", _players[0], _players[1]);
	QDebug dbg = qDebug();

	dbg << "Board: ";
	qDebug("(Board located at %p)", _board);
	for ( int i = 0; i != 14; i++ ) {
		dbg << " Field[" << i << "]: " << _board->getFieldBeans(i);
	}

	qDebug("Player on turn: %d, Player Last Turn: %d", _playerNextTurn, _playerLastTurn);
}

board *kMancalaHistoryItem::getBoard(void) {
	return _board; 
}

player **kMancalaHistoryItem::getPlayers(void) {
	return _players;
}

int kMancalaHistoryItem::getMove(void) {
	return _move; 
}

int kMancalaHistoryItem::getPlayerOnTurn(void) {
	return _playerNextTurn;
}

int kMancalaHistoryItem::getPlayerLastTurn(void) {
	return _playerLastTurn;
}
