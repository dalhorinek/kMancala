/* 
 * kMancala - ai.cpp 
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
#include <stdlib.h>

#include "ai.h"
#include "player.h"
#include <QApplication>
#include <time.h>

using namespace std;

int ai::_evalPosition(int player_id, board *copy) {
	int points_player_1 = copy->fields[TREASURY_PLAYER_1];
	int points_player_2 = copy->fields[TREASURY_PLAYER_2];

	for (int f = 0; f != 6; f++ ) {
		points_player_1 += copy->fields[PLAYER_1_FIRST_GAP+f];
		points_player_2 += copy->fields[PLAYER_2_FIRST_GAP+f];
	}
		
	return ( player_id == PLAYER_1 ? points_player_1 - points_player_2 : points_player_2 - points_player_1 );
}

int ai::_minimax(int player_id, board *copy, int depth) {
//	cout << id << ": Minimax Player Id: " << player_id << " Depth: " << depth << endl;

	qApp->processEvents();
	board *acopy;

	if ( depth <= 0 || !_controller->gameOn(copy) ) {
//		cout << "Evaluating position: Player Id: " << player_id << endl;
		return _evalPosition(player_id, copy); 
	} else {
		int r = 0;
		int best_r = -0x0FFF;
	
		signed int minus = -1; 

		int next_player_id = (player_id == PLAYER_1 ? PLAYER_2 : PLAYER_1);

		if ( copy->isExtraMove() ) {
//			cout << id << ": Extra Move " << endl;
			copy->setExtraMove(0);
			next_player_id = player_id;
			minus = 1;
		}
	
		for (int i = 0; i != 6; i++) {
//			cout << id << ": Move: " << i << endl;
			
			if ( _controller->checkMove(next_player_id, copy, i) ) {
//				cout << id << ": Move is genuine " << endl;

				acopy = new board(copy);
				int res = acopy->doMove(next_player_id, i);
				r = minus*_minimax(next_player_id, acopy, depth-1);
				if ( res == STATUS_CAPTURE ) r += 10;
			
//				cout << id << ": Value: " << r << ( res == STATUS_CAPTURE ? " CAPTURE " : "" ) << endl;

				delete acopy;
				best_r = ( best_r < r ? r : best_r );
			}
		}
		return best_r;
	}
}

ai::ai(controller *controller) {
	_controller = controller;
	_level = 0;
}

int ai::getMove(int player_id, board *p_board) {
	int best_move = 0; 
	int best_points = -0x0FFF;
	int r = 0;

	board *copy; 

	qDebug("Ai level: %d", _level);
	for ( int i = 0; i != 6; i++ ) {
		cout << "FIRST LEVEL: Move: " << i << endl;
		if ( _controller->checkMove(player_id, p_board, i) ) {
			copy = 	new board(p_board);
			int result = copy->doMove(player_id, i);
			r = _minimax(player_id, copy, _level);
			if ( result == STATUS_CAPTURE ) {
				r += 10;
			}

			qDebug("Value: %d", r);
			delete copy;

			if ( r > best_points ) {
				best_points = r; 
				best_move = i;
			}
		}
	}
	
	return best_move;
}

void ai::setLevel(int level) {
	_level = level;
}
