/* 
 * kMancala - board.h 
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
#ifndef BOARD_H
#define BOARD_H

#define TREASURY_PLAYER_1 6
#define TREASURY_PLAYER_2 13
#define FIELDS 14
#define PLAYER_1_FIRST_GAP 0 
#define PLAYER_2_FIRST_GAP 7

#define STATUS_EXTRA_MOVE 1
#define STATUS_CAPTURE -1
#define STATUS_OK 0
#define BEANS 4

class board {
	private:
		int _extraMove;
	public:
		int fields[14];

		board(void);
		board(const board *b);
		
		int getFieldBeans(int i);
		void setFieldBeans(int i, int beans);
		void addFieldBean(int i);
		bool isExtraMove(void);
		void setExtraMove(int b);
		
		int nextField(int field);
		int doMove(int player_id, int move);
		void moveToTreasuries(void);
};

#endif
