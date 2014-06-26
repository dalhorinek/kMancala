/* 
 * kMancala - player.cpp 
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
#include <iostream>

#include "player.h"

player::player(int id) {
	setId(id);
	setType(PLAYER_HUMAN);
	setName("Human Player");
	setLevel(0);
}

player::player(int id, QString s, int t, int level) {
	setId(id);
	setName(s);
	setType(t);
	setLevel(level);
}

player::player(int id, string s, int t, int level) {
	setId(id);
	setName(QString(s.c_str()));
	setType(t);
	setLevel(level);
}

player::player(player *p) {
	setId(p->getId());
	setName(p->getName());
	setType(p->getType());
	setLevel(p->getLevel());
}

void player::setName(QString s) {
	_name = s; 
}

void player::setType(int t) {
	if ( t == PLAYER_HUMAN || t == PLAYER_AI ) {
		_type = t; 
	} else {
		cerr << "Bad player type!" << endl;
		return; 
	}
}

void player::setLevel(int level) {
	_level = level;
}

void player::setId(int id) {
	_id = id;
}

int player::getType(void) {
	return _type;
}

int player::getId(void) {
	return _id;
}

int player::getLevel(void) {
	return _level;
}

QString player::getName(void) {
	return _name;
}
