/* 
 * kMancala - board.cpp 
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
#include "board.h"
#include "player.h"

using namespace std;

/*
 *     12 11 10  9  8  7 
 *  13                    6
 *      0  1  2  3  4  5
 */
board::board(void) {
	// initialize board 

	int i = 0;
	
	for (i = 0; i != TREASURY_PLAYER_1; i++) {
		fields[i] = BEANS;
	}

	for (i = 7; i != TREASURY_PLAYER_2; i++) {
		fields[i] = BEANS; 
	}


	fields[TREASURY_PLAYER_1] = 0;
	fields[TREASURY_PLAYER_2] = 0;

	_extraMove = false;
}

board::board(const board *b) {
	if ( b == NULL ) return; 

	for (int i = 0; i != FIELDS; i++) {
		fields[i] = b->fields[i];
	}
}

int board::getFieldBeans(int i) {
  if ( i >= 0 && i < FIELDS ) {
      return fields[i];
  }

  return 0;
}

bool board::isExtraMove() {
	return _extraMove;
}

void board::setExtraMove(int b) {
	_extraMove = b;
}

void board::setFieldBeans(int i, int beans) {
  if ( i >= 0 && i < FIELDS )
      fields[i] = beans;
}

void board::addFieldBean(int i) {
  if ( i >= 0 && i < FIELDS )
      fields[i]++;
}

int board::nextField(int field) {
	return ( field >= 13 ? 0 : field+1 );
}

int board::doMove(int player_id, int move) {
	int field = (player_id == PLAYER_2 ? move+PLAYER_2_FIRST_GAP : move );
	int opponents = ( player_id == PLAYER_2 ? TREASURY_PLAYER_1 : TREASURY_PLAYER_2 ); 

	int stones = fields[field];
	fields[field] = 0; 

	// redistribute stones
	for ( int i = 0; i != stones; i++ ) {
		field = nextField(field);
		if ( field == opponents ) field = nextField(field); 
		fields[field]++;
	}

	int treasury = ( player_id == PLAYER_1 ? TREASURY_PLAYER_1 : TREASURY_PLAYER_2 );
	int field_start = ( player_id == PLAYER_1 ? PLAYER_1_FIRST_GAP : PLAYER_2_FIRST_GAP );

	if ( field == treasury ) {
		_extraMove = 1;
		return STATUS_EXTRA_MOVE;
	} else if ( field >= field_start && field < field_start+6 && fields[field] == 1 ) {
		// capture
		fields[treasury] += fields[12-field] + 1;
		fields[12-field] = fields[field] = 0;
		return STATUS_CAPTURE;
	}

	return STATUS_OK;
}

void board::moveToTreasuries(void) {
	for (int i = 0; i < 6; i++) {
		fields[TREASURY_PLAYER_1] += fields[PLAYER_1_FIRST_GAP+i];
		fields[PLAYER_1_FIRST_GAP+i] = 0;

		fields[TREASURY_PLAYER_2] += fields[PLAYER_2_FIRST_GAP+i];
		fields[PLAYER_2_FIRST_GAP+i] = 0;
	}
}
