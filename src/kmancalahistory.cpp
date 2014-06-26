/* 
 * kMancala - kmancalahistory.cpp 
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

#include <QListWidget>
#include <QList>

#include "kmancalahistory.h"
#include "kmancalahistoryitem.h"

kMancalaHistory::kMancalaHistory(QWidget *parent = 0)
  : QListWidget(parent) {
}

void kMancalaHistory::init(player **p, board *b) {
	clear();
	qDebug("History cleared");

	player **cplayers = new player*[2]; 
	cplayers[PLAYER_1] = new player(p[PLAYER_1]); 
	cplayers[PLAYER_2] = new player(p[PLAYER_2]); 

	qDebug("Adding first item");
	
	addItem(new kMancalaHistoryItem(this, new board(b), cplayers, 0, 0, -1));
}

void kMancalaHistory::init(QList<kMancalaHistoryItem*> history) {
	clear();
	int i = 0;

	while ( !history.isEmpty() ) {
		insertItem(i, history.takeFirst());
		i++;
	}
}

void kMancalaHistory::addState(player **p, board *b, int playerLastTurn, int playerNextTurn, int move) {
	player **cplayers = new player*[2]; 
	cplayers[PLAYER_1] = new player(p[PLAYER_1]);
	cplayers[PLAYER_2] = new player(p[PLAYER_2]); 

	kMancalaHistoryItem *n = new kMancalaHistoryItem(this, 
			new board(b), cplayers, playerLastTurn, playerNextTurn, move);

	addItem(n);
}

void kMancalaHistory::removeState(int id) {
	QListWidgetItem *i = item(id);
	takeItem(id);
	delete i;
}


kMancalaHistoryItem *kMancalaHistory::getState(int id) {
	QListWidgetItem *i = item(id);
	qDebug("Getting item: %d", id);
	return (kMancalaHistoryItem*) i;
}

kMancalaHistoryItem *kMancalaHistory::getLast(void) {
	int cnt = count();

	if ( cnt > 0 ) 
		return getState(cnt-1);

	return NULL;
}

QList<kMancalaHistoryItem*> kMancalaHistory::dump(void) {
	QList<kMancalaHistoryItem*> list;
	for (int i = 0; i != count(); i++) {
		list.push_back(getState(i));
	}

	return list;
}
