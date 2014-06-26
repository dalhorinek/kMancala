/* 
 * kMancala - ai.h
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
#ifndef AI_H
#define AI_H

#include "board.h"
#include "controller.h"

class ai {
	private:
		controller *_controller;
		int _level;
		int _evalPosition(int player_id, board *copy);
		int _minimax(int player_id, board *copy, int depth);
	public:
		ai(controller *controller);
		int getMove(int player_id, board *_board);
		void setLevel(int level);
};
#endif
