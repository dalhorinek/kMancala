/* 
 * kMancala - player.h
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
#include <string>
#include <QString>

#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_1 0 
#define PLAYER_2 1 

#define PLAYER_HUMAN 1
#define PLAYER_AI 2

using namespace std;

typedef struct {
    QString name;
    int type;
    int ai_level;
} player_settings;

class player {
	private:
		int _id;
		QString _name;
		int _type;
		int _level;
	public:
		player(int id);
		player(int id, string s, int t, int level = 0);
		player(int id, QString s, int t, int level = 0);
		player(player *p);
		void setName(QString s);
		void setType(int t);	
		void setId(int id);
		void setLevel(int level);
		int getType(void);
		int getId(void);
		int getLevel(void);
		QString getName(void);
		
};
#endif
