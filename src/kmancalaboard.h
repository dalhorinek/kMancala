/* 
 * kMancala - kmancalaboard.h
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
#ifndef KMANCALABOARD_H
#define KMANCALABOARD_H

#include <QSvgRenderer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSignalMapper>

#include "kmancalarenderer.h"
#include "kmancalaboardgap.h"

#include "ai.h"
#include "board.h"
#include "controller.h"
#include "player.h"

#define BORDER 30
#define GAP_TREASURY_WIDTH 80
#define GAP_TREASURY_HEIGHT 170
#define GAP_WIDTH 75
#define GAP_HEIGHT 65
#define BEAN_PIXMAP_WIDTH 7
#define BEAN_PIXMAP_HEIGHT 7
#define TOP 0.25
#define BOTTOM 0.75

/*
 * Board layout scheme
 * --------------------
 *
 * Player 2
 *                field[12] field[11] field[10] field[9] field[8] field[7]
 *   field[13]                                                              field[6] (Treasury Player 1)
 *                field[0]  field[1]  field[2]  field[3] field[4] field[5]
 *                                                                               Player 1
 */

class kMancalaBoard : public QGraphicsView
{
    Q_OBJECT
public:
	kMancalaBoard(QSize dim, QWidget *parent);
	~kMancalaBoard();

	void init(board *b, player *p[2], int currentPlayer);
	void setSize(QSize s);
	void drawBackground(QPainter *painter, const QRectF &rect);
	void setClickable(bool clickable);

/* events */
    void resizeEvent(QResizeEvent *event);
    
private:
	QGraphicsScene *m_scene;
	kMancalaRenderer *m_renderer;

	QSignalMapper *sm;
	kMancalaBoardGap *fields[14];
	QGraphicsPixmapItem *fields_scoreBox[14];
	QGraphicsTextItem *fields_scoreBox_score[14];
	QGraphicsTextItem *fields_scoreBox_id[14];

	QGraphicsTextItem *_players[2];
	QGraphicsPixmapItem *_playerTypes[2];

	QGraphicsTextItem *_pauseItem;
	QGraphicsPixmapItem *_pauseBackItem;

	QGraphicsTextItem *_gameOverItem;
	QGraphicsTextItem *_gameOverResultItem;
	QGraphicsItem *_gameOverTextBackItem;

	int _boardInitialized;
	bool _clickable;

	void drawBoard(void);
	void drawBeans(board *b);
	void drawPlayers(player **p);
	void drawHilightPlayer(int player, QColor color = QColor("white"));
	void drawHilightGap(int i, QColor color = QColor("white"));

public slots:
	void updateBoard(board *b);
	void updatePlayers(player **p, int currentPlayer);
	void playerSwitched(player *p);
	void gameOver(player *p_winner);
	void clearGameOver(void);
	void click(const int i);
	void pause(void);
	void play(void);
	void hint(int i, int currentPlayer);

signals:
	void playerHumanMove(int i);
	void playerAIMove(void);
};

QPointF countPoint(QPointF point, double angle);
#endif // KMANCALABOARD_H
