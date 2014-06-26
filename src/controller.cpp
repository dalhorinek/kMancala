/* 
 * kMancala - controller.cpp 
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
#include "controller.h"
#include "board.h"
#include "player.h"

using namespace std;

controller::controller() {
    _currentPlayer = 0;
}

bool controller::checkMove(int player_id, board *b, int move) {
	/* move has to be 0 - 5 */

	int field = 0;
	if ( move >= 0 && move < 6 )
	    field = move + ( player_id == PLAYER_1 ? PLAYER_1_FIRST_GAP : PLAYER_2_FIRST_GAP );
	else
	    return false;

	if ( b->fields[field] != 0 )
	    return true;
	
	return false;
}

bool controller::gameOn(board *b) {
	int count_1 = 0; 
	int count_2 = 0;

	for ( int i = 0; i != 6; i++ ) {
		count_1 += b->fields[i];
		count_2 += b->fields[PLAYER_2_FIRST_GAP+i];
	}

	if ( count_1 == 0 || count_2 == 0 )
		return false;
	else 
		return true;
}

void controller::setCurrentPlayer(int i) {
	if ( i == PLAYER_1 || i == PLAYER_2 ) {
		_currentPlayer = i;
	}
}

void controller::switchPlayer(void )
{
	_currentPlayer = !_currentPlayer;
}

int controller::currentPlayer(void )
{
	return _currentPlayer;
}

int controller::winner(board *b) {

	if ( b->fields[TREASURY_PLAYER_1] > b->fields[TREASURY_PLAYER_2] ) {
	    return 0;
	} else if ( b->fields[TREASURY_PLAYER_1] < b->fields[TREASURY_PLAYER_2] ) {
	    return 1;
	} else {
	    return -1;
	}
}
