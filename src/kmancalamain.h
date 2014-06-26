/* 
 * kMancala - kmancalamain.h
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
#ifndef KMANCALAVIEW_H
#define KMANCALAVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QList>

#include "kmancalaboard.h"
#include "kmancalahistory.h"
#include "kmancalahistoryitem.h"
#include "controller.h"
#include "board.h"
#include "player.h"

typedef struct {
	QList<kMancalaHistoryItem*> history;
	int game;
} GAME_STATE;

class kMancalaMain : public QWidget {
    Q_OBJECT
public:
	kMancalaMain(QSize s, QWidget *parent);
	~kMancalaMain();
	void startGame(player **players);
	void loadGame(QList<kMancalaHistoryItem*> history);
	void stopGame();
	bool gameActive();
	bool gamePaused();
	player *getPlayer(int id);
	void updatePlayers(player **p);
	GAME_STATE *getGameState(void);

private:
	kMancalaBoard *_guiBoard;
	QLabel *_historyLabel; 
	kMancalaHistory *_historyListWidget;
	    
	ai *_ai;
	board *_board;
	controller *_controller;
	player **_players;

	QTimer *_playerAiMoveTimer; 

	bool _gameActive;
	bool _gameOver;
	bool _gamePaused; 

	void _init(player **players);
	void _startGame(board *b, int playerOnMove);
	void _playerMove(int currentPlayer, int move);
	void _manageToolbarItems(void);

public slots:
	void playerHumanMove(int i);
	void playerAIMove();
	void startPlayerAIMove();
	void toggleHistory(bool checked);
	void historyShow(int currentRow);
	void historyUndo(void);
	void historyRedo(void);
	void pauseGame(void);
	void unPauseGame(void);
	void hint(void);

signals:
	void playersUpdated(player **p, int currentPlayer);
	void updateBoard(board *b);
	void playerSwitched(player *p);
	void gameOver(player *p);
	void clearGameOver();
	void gamePause();
	void gameUnPause();
	void playerExtraMove();
	void playerCapture();
	void statusShowMessage(QString message, int timeout);
	void suggestHint(int id, int currentPlayer);
	void enableHint();
	void disableHint();
	void enableUndo();
	void disableUndo();
	void enableRedo();
	void disableRedo();
};

#endif // kMancalaVIEW_H
