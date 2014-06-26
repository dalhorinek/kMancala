/* 
 * kMancala - kmancalamain.cpp 
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
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QSvgRenderer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBrush>
#include <QPixmap>
#include <QPainter>
#include <QApplication>
#include <QListWidget>
#include <QTimer>

#include <klocale.h>

#include "kmancalamain.h"
#include "kmancalaboard.h"
#include "kmancalaprefs.h"
#include "kmancalahistory.h"

#include "player.h"

kMancalaMain::kMancalaMain(QSize s, QWidget *parent = 0)
  : QWidget(parent) {
	QHBoxLayout *layout = new QHBoxLayout();
	QVBoxLayout *historyBox = new QVBoxLayout();

	_historyLabel = new QLabel(i18n("<h3 style=\"color: black;\">Moves History</h3>"));
	_historyListWidget = new kMancalaHistory(this);
	_historyListWidget->setFocusPolicy(Qt::NoFocus);

	historyBox->addWidget(_historyLabel);
	historyBox->addWidget(_historyListWidget);

	_guiBoard = new kMancalaBoard(s, this);
	_gameActive = false;
	_gameOver = false;
	_gamePaused = false;

	setBackgroundRole(QPalette::Dark);
	layout->addWidget(_guiBoard);
	layout->addLayout(historyBox);

	setLayout(layout);
	setMinimumSize(s);
	toggleHistory(false);

	connect(_guiBoard, SIGNAL(playerHumanMove(int)), this, SLOT(playerHumanMove(int)));
	connect(_guiBoard, SIGNAL(playerAIMove()), this, SLOT(playerAIMove()));
	connect(_historyListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(historyShow(int)));

	connect(this, SIGNAL(updateBoard(board*)), _guiBoard, SLOT(updateBoard(board*)));
	connect(this, SIGNAL(playerSwitched(player *)), _guiBoard, SLOT(playerSwitched(player *)));
	connect(this, SIGNAL(gameOver(player*)), _guiBoard, SLOT(gameOver(player*)));
	connect(this, SIGNAL(playersUpdated(player**, int)), _guiBoard, SLOT(updatePlayers(player**, int)));
	connect(this, SIGNAL(gamePause()), _guiBoard, SLOT(pause()));
	connect(this, SIGNAL(gameUnPause()), _guiBoard, SLOT(play()));
	connect(this, SIGNAL(suggestHint(int, int)), _guiBoard, SLOT(hint(int, int)));
	connect(this, SIGNAL(clearGameOver()), _guiBoard, SLOT(clearGameOver()));

	_controller = new controller;
	_ai = new ai(_controller);
	_playerAiMoveTimer = new QTimer;
	_playerAiMoveTimer->setSingleShot(true);
	connect(_playerAiMoveTimer, SIGNAL(timeout()), this, SLOT(startPlayerAIMove()));

	_board = NULL; 
	_players = NULL;
}

kMancalaMain::~kMancalaMain() {
}

void kMancalaMain::_init(player **players) {
	if ( _board != NULL ) delete _board;
	if ( _players != NULL ) delete _players;

	_players = new player*[2]; 

	_players[PLAYER_1] = players[PLAYER_1];
	_players[PLAYER_2] = players[PLAYER_2]; 

	if ( _players[PLAYER_1]->getType() == PLAYER_AI && _players[PLAYER_2]->getType() == PLAYER_AI ) 
		emit disableHint();
	else 
		emit enableHint();	
}

void kMancalaMain::_startGame(board *b, int playerOnMove) {
	_board = b;
	_guiBoard->init(b, _players, playerOnMove);
	_controller->setCurrentPlayer(playerOnMove);

	emit statusShowMessage(
		i18n("New game created") + \
		 " | " + i18n("Player ") + \
		_players[playerOnMove]->getName() + \
		i18n(" is on move"), 
	6000);

	_gameActive = true;
	_gamePaused = false;

	if ( _controller->gameOn(_board) ) {
		_gameOver = false;

		if ( _players[playerOnMove]->getType() == PLAYER_AI ) {
			playerAIMove();
		}
	} else {
		_gameOver = true;
	}

	qDebug("Game started ... ");
}

void kMancalaMain::startGame(player **players) {
	_init(players);
	board *b = new board;

	_historyListWidget->init(players, b);
	_startGame(b, PLAYER_1);
	emit disableRedo();
	emit disableUndo();
}

void kMancalaMain::loadGame(QList<kMancalaHistoryItem*> history) {
	_historyListWidget->init(history);
	kMancalaHistoryItem *last = history.last();

	qDebug("loading game ... ");
	if ( last ) {
		_init(last->getPlayers());
		_startGame(last->getBoard(), last->getPlayerOnTurn());

		if ( !_controller->gameOn(_board) )
			_historyListWidget->setCurrentRow(_historyListWidget->count()-1);

		emit disableRedo();

		if ( _historyListWidget->count() > 1 ) 
			emit enableUndo();

		qDebug("Game loaded");
	} else {
		qDebug("Cannot load game, history missing");
	}
}

void kMancalaMain::stopGame() {
	_gameOver = true;
	emit disableHint();
}

bool kMancalaMain::gameActive() {
	return _gameActive;
}

bool kMancalaMain::gamePaused() {
	return _gamePaused;
}

player *kMancalaMain::getPlayer(int id) {
	if ( _players != NULL ) {
		return _players[id];
	}

	return NULL; 
}

void kMancalaMain::updatePlayers(player *p[2]) {
	qDebug("kMancalaMain::updatePlayers(...): Updating players ... ");

	_players[PLAYER_1] = p[PLAYER_1];
	_players[PLAYER_2] = p[PLAYER_2];

	if ( _players[PLAYER_1]->getType() == PLAYER_AI && _players[PLAYER_2]->getType() == PLAYER_AI )
		emit disableHint();
	else 
		emit enableHint();

	qDebug() << "Player 1: " << _players[PLAYER_1]->getName() << " Type: " << _players[PLAYER_1]->getType() << " Level: " << _players[PLAYER_1]->getLevel();
	qDebug() << "Player 2: " << _players[PLAYER_2]->getName() << " Type: " << _players[PLAYER_2]->getType() << " Level: " << _players[PLAYER_2]->getLevel();

	qDebug("[1] (_players) Players: %p, Player 1: %p, Player 2: %p", _players, _players[0], _players[1]);
	qDebug("[2] (p) Players: %p, Player 1: %p, Player 2: %p", p, p[0], p[1]);

	int currentPlayer = _controller->currentPlayer();

	qDebug("Current Player: %d", currentPlayer);

	qDebug("Sending playersUpdated signal ... ");
	emit playersUpdated(_players, currentPlayer);
}

GAME_STATE *kMancalaMain::getGameState(void) {
	GAME_STATE *status = new GAME_STATE;

	status->history = _historyListWidget->dump(); 
	status->game = ( _gameOver ? 1 : 0 ); 

	return status;
}

/* private */

void kMancalaMain::_playerMove(int currentPlayer, int move) {
	if ( _gamePaused ) { qDebug("_playerMove(): Game paused, halting player move"); return; }
	if ( !_gameActive ) { qDebug("_playerMove(): Game not active, halting player move"); return; }
	if ( _gameOver ) { qDebug("_playerMove(): Game over, halting player move"); return; }

	int result = _board->doMove(currentPlayer, move);

	if ( !_controller->gameOn(_board) ) {
		_board->moveToTreasuries();
		emit updateBoard(_board);

		int winner = _controller->winner(_board);

		player *p_winner = NULL;
		qDebug("Winner is %d", winner);
		
		if ( winner != -1 )
			p_winner = _players[winner];

		qDebug("Game Over");
		_historyListWidget->addState(_players, _board, currentPlayer, 0, move);

		emit gameOver(p_winner);
		stopGame();
		return;
	}

	player *current = _players[currentPlayer];
	QString playerName = current->getName();

	emit updateBoard(_board);

	switch (result) {
		case STATUS_EXTRA_MOVE:
			emit playerExtraMove();
			emit statusShowMessage(i18n("Player ") + _players[currentPlayer]->getName() + i18n(" Extra move"), 6000);
			_historyListWidget->addState(_players, _board, currentPlayer, currentPlayer, move);
			_board->setExtraMove(0); // clear extra move 

			if ( _players[currentPlayer]->getType() == PLAYER_AI ) {
				playerAIMove();
			}
			break;
		case STATUS_CAPTURE:
			emit statusShowMessage(i18n("Player ") + _players[currentPlayer]->getName() + i18n(" Capture"), 6000);
			emit playerCapture();
		case STATUS_OK:
			_controller->switchPlayer();
			currentPlayer = _controller->currentPlayer();
			player *p = _players[currentPlayer];
			
			if ( p != NULL && currentPlayer >= 0 && currentPlayer <= 1 ) {
				_historyListWidget->addState(_players, _board, !currentPlayer, currentPlayer, move);
				qApp->processEvents();

				emit playerSwitched(p);
			}
			
	}

	_manageToolbarItems();

}

void kMancalaMain::_manageToolbarItems() {
	if ( _historyListWidget->currentRow() > 0 || _historyListWidget->count() > 0 ) {
		emit enableUndo();
	} 

	if ( _historyListWidget->currentRow() == 0 ) {
		emit disableUndo();
	}

	if ( _historyListWidget->currentRow() >= 0 && (_historyListWidget->currentRow() < _historyListWidget->count()-1 )) {
		emit enableRedo();
	} else {
		emit disableRedo();
	}
}

/* slots */
void kMancalaMain::playerHumanMove(int i) {
	int currentPlayer = _controller->currentPlayer();

	if ( _players[currentPlayer]->getType() == PLAYER_AI ) 
		return;

	int move = ( currentPlayer == PLAYER_2 ? i-PLAYER_2_FIRST_GAP : i );

	qDebug("Move: %d, Current Player: %d \n", move, currentPlayer);
    
	if ( !_controller->checkMove(currentPlayer, _board, move) ) {
		qDebug("Invalid move");
		return;
	}
   
	_playerMove(currentPlayer, move); 
}

void kMancalaMain::playerAIMove() {
	_guiBoard->setClickable(false);

	qDebug("QTimer::singleShot, 800ms waiting ... ");
	_playerAiMoveTimer->start(1200);
	
}

void kMancalaMain::startPlayerAIMove() {
	if ( _gamePaused ) { qDebug("startPlayerAIMove(): Game paused, halting player move"); return; }
	if ( !_gameActive ) { qDebug("startPlayerAIMove(): Game not active, halting player move"); return; }

	qDebug("startPlayerAIMove(): starting ...");

	int currentPlayer = _controller->currentPlayer();
	qDebug("startPlayerAIMove(): Ai move current player %d", currentPlayer);

	int level = _players[currentPlayer]->getLevel();

	_ai->setLevel(level*2-1);
	qApp->processEvents();
	int r = _ai->getMove(currentPlayer, _board);

	if ( !_gameActive ) { qDebug("startPlayerAIMove(): Game not active, halting player move, test 2"); return; }
	_playerMove(currentPlayer, r);
}

void kMancalaMain::toggleHistory(bool checked) {
	if ( checked ) {
		_historyLabel->show();
		_historyListWidget->show();
	} else {
		_historyLabel->hide();
		_historyListWidget->hide();
	}
}

void kMancalaMain::historyShow(int currentRow) {
	if ( currentRow == -1 ) return; 

	_manageToolbarItems();

	if ( _historyListWidget->count() == 1 ) {
		_historyListWidget->setCurrentRow(-1);
		return;
	}

	qDebug("Restoring board ... to item: %d", currentRow);
	kMancalaHistoryItem *i = _historyListWidget->getState(currentRow);

	player **p = i->getPlayers();
	int currentPlayer = i->getPlayerOnTurn();

	_board = new board(i->getBoard());
	_players[0] = new player(p[0]);
	_players[1] = new player(p[1]);

	_controller->setCurrentPlayer(i->getPlayerOnTurn());

	emit playersUpdated(_players, currentPlayer);
	emit updateBoard(_board);

	emit clearGameOver();
	if ( !_controller->gameOn(_board) ) {
//		if ( _gameOver ) return;

		int winner = _controller->winner(_board);

		player *p_winner = NULL;
		qDebug("Winner is %d", winner);
		
		if ( winner != -1 )
			p_winner = _players[winner];


		qDebug("History Game Over");
		_guiBoard->play();
		qDebug("Emitting gameOver");
		emit gameOver(p_winner);
		stopGame();
		qDebug("historyShow done");
	} else {
		qDebug("Not game over ...");
		_gameOver = false;
		_gamePaused = false;
		pauseGame();
	}

}

void kMancalaMain::historyUndo(void) {
	if ( !_gameActive ) return;

	int row = _historyListWidget->currentRow();
	int count = _historyListWidget->count();

	if ( row == -1 ) row = count-1;
	qDebug("Row %d", row);

	if ( row-1 >= 0 ) {
		_historyListWidget->setCurrentRow(row-1);
	}
}

void kMancalaMain::historyRedo(void) {
	if ( !_gameActive ) return;

	int row = _historyListWidget->currentRow();
	int count = _historyListWidget->count();

	qDebug("Row %d", row);
	if ( row == -1 ) return;


	if ( count > row+1 ) {
		_historyListWidget->setCurrentRow(row+1);
	}
}

void kMancalaMain::pauseGame(void) {
	qDebug("pausing game ... ");
	if ( _gamePaused ) return;
	if ( !_gameActive ) return;
	if ( _gameOver ) return;

	_gamePaused = true;
	_playerAiMoveTimer->stop();
	_guiBoard->setClickable(false);
	emit disableHint();
	emit gamePause();
}

void kMancalaMain::unPauseGame(void) {
	if ( !_gamePaused ) return;
	if ( !_gameActive ) return;
	if ( _gameOver ) return;

	int row = _historyListWidget->currentRow();
	int count = _historyListWidget->count();

	qDebug("Count: %d, Row: %d", count, row);
	if ( row != -1 ) {
		for(int i = count; i != row+1; i--) {
			_historyListWidget->removeState(i-1);
		}

		_historyListWidget->setCurrentRow(-1);
		emit updateBoard(_board);
		emit updatePlayers(_players);
		qApp->processEvents();
	}

	qDebug("Current Row: %d, Count: %d", _historyListWidget->currentRow(), _historyListWidget->count());

	_gamePaused = false;
	_guiBoard->setClickable(true);
	emit gameUnPause();

	if ( _players[PLAYER_1]->getType() == PLAYER_AI && _players[PLAYER_2]->getType() == PLAYER_AI ) 
		emit disableHint();
	else 
		emit enableHint();	
	
	qApp->processEvents();

	if ( _players[_controller->currentPlayer()]->getType() == PLAYER_AI ) {
		playerAIMove();
	}
}

void kMancalaMain::hint(void) {
	if ( !_gameActive || _gamePaused || _gameOver ) return; 

	qDebug("Hinting hint ... ");

	int currentPlayer = _controller->currentPlayer();
	if ( _players[currentPlayer]->getType() == PLAYER_AI ) return;

	_ai->setLevel(5);
	int r = _ai->getMove(currentPlayer, _board);
	emit suggestHint(r, currentPlayer);
}


