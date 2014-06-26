/* 
 * kMancala - kmancalaboard.cpp 
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
#include <math.h>

#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QSignalMapper>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QList>
#include <QApplication>

#include <klocale.h>
#include <kicon.h>
#include <kstandarddirs.h>

#include "kmancalaboard.h"
#include "kmancalaboardgap.h"
#include "kmancalarenderer.h"

#include "ai.h"
#include "board.h"
#include "controller.h"
#include "player.h"

#define PI 3.1415
#define BEAN_RADIUS 8
#define PLAYER_TYPE_SIZE QSize(50,50)

#define GAP_SCORE_WIDTH GAP_WIDTH*0.7
#define GAP_SCORE_HEIGHT 15

kMancalaBoard::kMancalaBoard ( QSize dim = QSize(850, 310), QWidget *parent = 0 )
        : QGraphicsView (parent) {

	m_renderer 	= new kMancalaRenderer(this);
	m_scene 	= new QGraphicsScene();

	_boardInitialized = 0;
	_pauseItem = NULL; 
	_pauseBackItem = NULL;

	setScene ( m_scene );
	setSize(dim);
	setFixedSize(dim);

	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
	shadow->setOffset(3);
	shadow->setColor(QColor("black"));

	QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect();
	shadow2->setOffset(3);
	shadow2->setColor(QColor("black"));

	KIcon appIcon = KIcon("kmancala.png");

	QGraphicsItem *i;
	QPixmap icon = appIcon.pixmap(QSize(128,128));
 
	i = m_scene->addPixmap(icon);

	QPointF center = QPointF(m_scene->width()/2, m_scene->height()/2);
	i->setPos(center.x()-icon.width()/2, center.y()-icon.height()/2);
	i->setGraphicsEffect(shadow2);


	QGraphicsTextItem *text = new QGraphicsTextItem();
	text->setFont(QFont("Dejavu Sans", 14, false));
	text->setHtml("<h3 style=\"color: white;\">A board count-and-capture like game aka Kalah.</h3>");
	text->setTextWidth(630);
	text->setGraphicsEffect(shadow);


	m_scene->addItem(text);
	text->setPos(center.x()-text->textWidth()/2, center.y()+icon.height()+10);
	setClickable(true);
}

kMancalaBoard::~kMancalaBoard() {
}

void kMancalaBoard::init(board *b, player **p, int currentPlayer) {
	_boardInitialized = 0;
   
	sm = new QSignalMapper();
	_players[PLAYER_1] = NULL;
	_players[PLAYER_2] = NULL;
	_playerTypes[PLAYER_1] = NULL;
	_playerTypes[PLAYER_2] = NULL;
    
	_gameOverTextBackItem = NULL; 
	_gameOverItem = NULL; 
	_gameOverResultItem = NULL;

	_pauseItem = NULL;
	_pauseBackItem = NULL;

	drawBoard();
	_boardInitialized = 1;
    
	drawPlayers(p);
	drawHilightPlayer(currentPlayer);
	drawBeans(b);
	setClickable(true);
}

void kMancalaBoard::setClickable(bool clickable) {
	_clickable = clickable;
}

void kMancalaBoard::clearGameOver(void) {
	if ( _gameOverTextBackItem != NULL ) {
		m_scene->removeItem(_gameOverTextBackItem);
		m_scene->removeItem(_gameOverItem);
		m_scene->removeItem(_gameOverResultItem);

		_gameOverResultItem = NULL; 
		_gameOverItem = NULL; 
		_gameOverTextBackItem = NULL;
	}
}

void kMancalaBoard::drawBoard() {
	qDebug("drawBoard(): Adding items");

	m_scene->clear();

	qreal height = sceneRect().height();
	qreal width = sceneRect().width();    
	qreal middle = height/2;

	qDebug("Scene dimensions %lf x %lf", width, height);

	int treasury_width = GAP_TREASURY_WIDTH+BORDER;
	int gaps_space = width - 2*treasury_width;
	int gap_offset = (gaps_space-(6*GAP_WIDTH))/7;
	int left_offset = treasury_width + gap_offset;

	QPixmap treasury = m_renderer->renderPixmap("treasury", QSize(GAP_TREASURY_WIDTH, GAP_TREASURY_HEIGHT));
	QPixmap hole = m_renderer->renderPixmap("hole", QSize(GAP_WIDTH, GAP_HEIGHT));
	QPixmap scoreBox = m_renderer->renderPixmap("score_box", QSize(GAP_SCORE_WIDTH, GAP_SCORE_HEIGHT));
	QPixmap scoreBoxTreasury = m_renderer->renderPixmap("score_box_treasury", QSize(GAP_SCORE_WIDTH, GAP_SCORE_HEIGHT));

	for(int i = 0; i != 14; i++) {
		char id = 'a';
		switch (i) {
			case TREASURY_PLAYER_1:
			case TREASURY_PLAYER_2:
				fields[i] = new kMancalaBoardGap(treasury);
				fields_scoreBox[i] = new QGraphicsPixmapItem(scoreBoxTreasury);
				id = 'T';
				break;
			default:
				fields[i] = new kMancalaBoardGap(hole);
				fields_scoreBox[i] = new QGraphicsPixmapItem(scoreBox);

				QPointF holePos;
				if ( i >= 0 && i < 6 ) {
					id = 'a'+i; 
					holePos = QPointF(left_offset+(i*(GAP_WIDTH+gap_offset)), height*BOTTOM-GAP_HEIGHT);
				}
				else if ( i >= 7 && i < 13 ) {
					id = 'f' - (12-i);
					holePos = QPointF(left_offset+((12-i)*(GAP_WIDTH+gap_offset)), height*TOP);
				}

				fields[i]->setPos(holePos);
				fields_scoreBox[i]->setPos(QPointF(holePos.x()+(GAP_WIDTH-GAP_SCORE_WIDTH)/2, holePos.y()+GAP_HEIGHT-GAP_SCORE_HEIGHT/2));

				fields_scoreBox_id[i] = m_scene->addText(QString("%1").arg(id));
				fields_scoreBox_id[i]->setDefaultTextColor(QColor(200,200,200));
				fields_scoreBox_id[i]->setParentItem(fields_scoreBox[i]);
				fields_scoreBox_id[i]->setFont(QFont("Dejavu Sans", 9, QFont::Bold, false));
				fields_scoreBox_id[i]->setPos(GAP_SCORE_WIDTH/4-8,-5);

				connect(fields[i], SIGNAL(clicked()), sm, SLOT(map()));
				sm->setMapping(fields[i], i);
		}

		fields_scoreBox_score[i] = m_scene->addText(QString("%1").arg(0));
		fields_scoreBox_score[i]->setDefaultTextColor(QColor(255,255,255));
		fields_scoreBox_score[i]->setParentItem(fields_scoreBox[i]);
		fields_scoreBox_score[i]->setFont(QFont("Dejavu Sans", 9, QFont::Bold, false));
		fields_scoreBox_score[i]->setPos(GAP_SCORE_WIDTH/2+2,-5);

		m_scene->addItem(fields[i]);
		m_scene->addItem(fields_scoreBox[i]);
	}

	QPointF treasuryPos = QPointF(width-BORDER-GAP_TREASURY_WIDTH, middle-GAP_TREASURY_HEIGHT/2);
	fields[TREASURY_PLAYER_1]->setPos(treasuryPos);
	fields_scoreBox[TREASURY_PLAYER_1]->setPos(treasuryPos.x()+GAP_TREASURY_WIDTH/2-GAP_SCORE_WIDTH/2, treasuryPos.y()+GAP_TREASURY_HEIGHT*0.75);
	fields_scoreBox_score[TREASURY_PLAYER_1]->setPos(GAP_SCORE_WIDTH/2-9,-5);

	treasuryPos = QPointF(BORDER, middle-GAP_TREASURY_HEIGHT/2);
	fields[TREASURY_PLAYER_2]->setPos(treasuryPos);
	fields_scoreBox[TREASURY_PLAYER_2]->setPos(treasuryPos.x()+GAP_TREASURY_WIDTH/2-GAP_SCORE_WIDTH/2, treasuryPos.y()+GAP_TREASURY_HEIGHT*0.75);
	fields_scoreBox_score[TREASURY_PLAYER_2]->setPos(GAP_SCORE_WIDTH/2-9,-5);

	connect(sm, SIGNAL(mapped(const int &)), this, SLOT(click(const int &)));
}

QPointF _countPosition(QPointF p, double angle) {
	double rads = (angle*PI)/180.0f;

	double x = p.x()*cos(rads) - p.y()*sin(rads);
	double y = p.x()*sin(rads) + p.y()*cos(rads);

	return QPointF(x, y);
}

void kMancalaBoard::drawBeans(board* b) {
	if ( !_boardInitialized ) return;
	QPixmap bean = m_renderer->renderPixmap("bean", QSize(BEAN_PIXMAP_WIDTH, BEAN_PIXMAP_HEIGHT));

	for (int i = 0; i != 14; i++) {
		QPointF center;

		kMancalaBoardGap *cur = fields[i];
		int beans = b->getFieldBeans(i);

		if ( i == TREASURY_PLAYER_1 || i == TREASURY_PLAYER_2 ) {
			center.setX(GAP_TREASURY_WIDTH/2);
			center.setY(GAP_TREASURY_HEIGHT/2);
		} else {
			center.setX(GAP_WIDTH/2-BEAN_PIXMAP_WIDTH/2);
			center.setY(GAP_HEIGHT/2-BEAN_PIXMAP_HEIGHT/2);
		}
		    
		QList<QGraphicsItem*> items = cur->childItems();

		for (int p = 0; p != items.size(); p++) {
			m_scene->removeItem(items[p]);
		}

		int circle = 0; 
		double angle = 0;
		double angle_delta = 0;
		QPointF point = center;
		QGraphicsItem *grItem;

		for (int j = 0; (j != beans && j != 37) ; j++) {
			grItem = m_scene->addPixmap(bean);
			grItem->setParentItem(cur);
			grItem->setPos(point);

			if (angle <= 0 || (angle + angle_delta) >= 360 ) {
				circle++; 
				angle = 0;
				angle_delta = 360/(circle*PI*2);
			}

			QPointF rotation = _countPosition(QPointF(0, circle*BEAN_RADIUS), angle);
			point = QPointF(center.x()+rotation.x(), center.y()+rotation.y());
			angle += angle_delta; 
		}

		fields_scoreBox_score[i]->setPlainText(QString().setNum(beans));
	}
}

void kMancalaBoard::drawPlayers(player **p) {
    if ( !_boardInitialized ) return;

	qDebug("Drawing players ... ");

	// clear scene first even if there is nothing, then nothing happens
	QPixmap player_ai = m_renderer->renderPixmap("player_computer", PLAYER_TYPE_SIZE);
	QPixmap player_human = m_renderer->renderPixmap("player_human", PLAYER_TYPE_SIZE);

	if ( _players[PLAYER_1] == NULL ) {
		qDebug("Adding players[0]");

		_players[PLAYER_1] = new QGraphicsTextItem;
		_playerTypes[PLAYER_1] = new QGraphicsPixmapItem;

		m_scene->addItem(_players[PLAYER_1]);
		m_scene->addItem(_playerTypes[PLAYER_1]);
	}

	if ( _players[PLAYER_2] == NULL ) {
		qDebug("Adding players[1]");

		_players[PLAYER_2] = new QGraphicsTextItem;
		_playerTypes[PLAYER_2] = new QGraphicsPixmapItem;

		m_scene->addItem(_players[PLAYER_2]);
		m_scene->addItem(_playerTypes[PLAYER_2]);
	}


	for (int i = 0; i != 2; i++)  {
		_players[i]->setPlainText(p[i]->getName());
		_players[i]->setDefaultTextColor(QColor("black"));
		_players[i]->setFont(QFont("Dejavu Sans", 15, QFont::Bold, false));

		QRectF playerTextDim = _players[i]->boundingRect();

		switch (p[i]->getType()) {
			case PLAYER_AI:
				_playerTypes[i]->setPixmap(player_ai);
				break;
			default:
				_playerTypes[i]->setPixmap(player_human);
		}

		QRectF playerTypeDim = _playerTypes[i]->boundingRect();
		QPointF playerPos; 
		QPointF playerTypePos;
		QSize playerDim = QSize(playerTypeDim.width()+playerTextDim.width()+BORDER, playerTypeDim.height()+BORDER);

		switch (i) {
			case PLAYER_1:
				playerTypePos = QPointF(
					m_scene->width()-playerTypeDim.width()-BORDER,
					m_scene->height()-playerTypeDim.height()-BORDER
				);
				playerPos = QPointF(
					playerTypePos.x()-playerTextDim.width()-20,
					playerTypePos.y()+playerTypeDim.height()/2-playerTextDim.height()/2
				);
				break;
			case PLAYER_2:
				playerTypePos = QPointF(BORDER, BORDER);
				playerPos = QPointF(
					playerTypePos.x()+playerTypeDim.width()+20,
					playerTypePos.y()+playerTypeDim.height()/2-playerTextDim.height()/2
				);
		}

		_players[i]->setPos(playerPos);
		_playerTypes[i]->setPos(playerTypePos);
	}

	qDebug("Players drawed");
}

void kMancalaBoard::drawHilightPlayer(int player, QColor color) {
	_players[!player]->setGraphicsEffect(NULL);
	_players[!player]->setOpacity(0.8);
	_players[!player]->setDefaultTextColor(QColor("black"));

	_players[player]->setOpacity(1);
	_players[player]->setDefaultTextColor(color);

	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
	effect->setParent(_players[player]);
	effect->setBlurRadius(10);
	effect->setColor(QColor("white"));
	effect->setOffset(0);
	_players[player]->setGraphicsEffect(effect);
}

void kMancalaBoard::drawHilightGap(int i, QColor color) {
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
	effect->setParent(fields[i]);
	effect->setBlurRadius(10);
	effect->setColor(color);
	effect->setOffset(0);
	fields[i]->setGraphicsEffect(effect);
}

void kMancalaBoard::drawBackground(QPainter *painter, const QRectF &rect) {
	QRectF r(0, 0, m_scene->width(), m_scene->height());

	qDebug("drawBackground(): Drawing background");
	qDebug("width: %lf, height: %lf\n", rect.width(), rect.height());

	QColor bg = QColor(10,10,10);
	painter->fillRect(-5, -5, m_scene->width()+10, 20, bg);			// [top]
	painter->fillRect(-5, -5, 20, m_scene->height()+10, bg);		// [left]
	painter->fillRect(-5, m_scene->height()-10, m_scene->width()+10, 20, bg); // [bottom]
	painter->fillRect(m_scene->width()-10, -5, 20, m_scene->height(), bg); // [right]

	if ( m_renderer->elementExists ( QString ( "board" ) ) ) {
		qDebug ("drawBackground(): -> Rendering" );
		m_renderer->render(painter, QString("board"), r);
	} else {
		qDebug ("drawBackground(): -> No background found" );
	}

	QGraphicsView::drawBackground(painter, rect);
}

void kMancalaBoard::resizeEvent(QResizeEvent *event) {
    qDebug("resizeEvent(): Resize event");
    QSize s = size();

    setSize(s);
    
    qDebug("resizeEvent(): -> Scene size now %d x %d", s.width(), s.height());
    QGraphicsView::resizeEvent(event);
}

void kMancalaBoard::setSize(QSize s) {
    qDebug("setSize(): Setting size");
    qDebug("setSize(): -> Setting scene to %d x %d", s.width(), s.height());
    m_scene->setSceneRect(QRectF(0,0,s.width()-10,s.height()-10));
    
    QRectF scene_rect = m_scene->sceneRect();
    qDebug("setSize(): -> Scene rectangle: x: %lf, y: %lf, width: %lf, height: %lf", scene_rect.x(), scene_rect.y(), scene_rect.width(), scene_rect.height());
}

/* slots */
void kMancalaBoard::click(const int i) {
	if ( _clickable ) {
		for (int f = 0; f != 14; f++) {
			if ( fields[f]->graphicsEffect() ) {
				fields[f]->setGraphicsEffect(NULL);
			}
		}

		emit playerHumanMove(i);
	} else 
		qDebug("Unclickable");
}

void kMancalaBoard::updateBoard(board* b) {
	qDebug("Updating board");
	drawBeans(b);
	qDebug("Updated");
}

void kMancalaBoard::updatePlayers(player **p, int currentPlayer) {
	qDebug("kMancalaBoard::updatePlayers(): Updating players");
	qDebug("Players: %p, Player 1: (%p)%p, Player 2: (%p)%p", p, *(p+0), p[0], *(p+1), p[1]);

	drawPlayers(p);
	drawHilightPlayer(currentPlayer);
	qDebug("Updated");
}

void kMancalaBoard::playerSwitched(player *p) {
	qDebug("Switching players");
	drawHilightPlayer(p->getId());
	qDebug("Switched");
	if ( p->getType() == PLAYER_AI ) {
		setClickable(false);
		emit playerAIMove();
	} else {
		setClickable(true);
	}

}

void kMancalaBoard::gameOver(player *p) {
	qDebug(" Game Over ... Inform user ");

	if ( _gameOverResultItem != NULL ) return; 

	QString textGameOver = i18n("GameOver");
	QString textResult;

	if ( p == NULL ) {
		qDebug("It's a Tie");
		textResult = i18n("It's a tie");
	} else {
		qDebug("Winner is a player");
		drawHilightPlayer(p->getId(), QColor("red"));
		textResult = i18n("Winner is %1").arg(p->getName());
	}

	QGraphicsTextItem *_gameOverResultItem = new QGraphicsTextItem(textResult);
	_gameOverResultItem->setFont(QFont("Dejavu Sans", 15, QFont::Bold, false));
	_gameOverResultItem->setDefaultTextColor(QColor("white"));

	_gameOverItem = new QGraphicsTextItem(textGameOver);
	_gameOverItem->setFont(QFont("Dejavu Sans", 18, QFont::Bold, false));
	_gameOverItem->setDefaultTextColor(QColor("red"));

	QRectF resultDim = _gameOverResultItem->boundingRect();
	QRectF gameOverDim = _gameOverItem->boundingRect();

	int width = ( resultDim.width() > gameOverDim.width() ? resultDim.width() : gameOverDim.width() );
	int height = resultDim.height() + gameOverDim.height();

	QPixmap textBack = m_renderer->renderPixmap("text_background", QSize(width+30, height+20));

	_gameOverTextBackItem = m_scene->addPixmap(textBack);
	QRectF textBackDim = _gameOverTextBackItem->boundingRect();

	_gameOverTextBackItem->setPos(m_scene->width()/2-textBackDim.width()/2, m_scene->height()/2-textBackDim.height()/2);
	_gameOverItem->setParentItem(_gameOverTextBackItem);
	_gameOverItem->setPos(textBackDim.width()/2 - gameOverDim.width()/2, 5);
	_gameOverResultItem->setParentItem(_gameOverTextBackItem);
	_gameOverResultItem->setPos(textBackDim.width()/2 - resultDim.width()/2, gameOverDim.height()+5);

	m_scene->addItem(_gameOverItem);
	m_scene->addItem(_gameOverResultItem);
}

void kMancalaBoard::pause() {
	qDebug("Show pause");

	QPointF center(m_scene->width()/2, m_scene->height()/2);

	if ( _pauseItem != NULL ) return; // pause item is already created, stop here 

	_pauseItem = new QGraphicsTextItem(i18n("Paused"));
	_pauseItem->setFont(QFont("Dejavu Sans", 15, QFont::Bold, false));
	_pauseItem->setDefaultTextColor(QColor("white"));

	QRectF pauseTextDim = _pauseItem->boundingRect();

	int width = pauseTextDim.width();
	int height = pauseTextDim.height();

	_pauseBackItem = new QGraphicsPixmapItem(m_renderer->renderPixmap("text_background", QSize(width+30, height+20)));
	QRectF pauseBackDim = _pauseBackItem->boundingRect();

	_pauseBackItem->setPos(center.x()-pauseBackDim.width()/2, center.y()-pauseBackDim.height()/2);

	_pauseItem->setParentItem(_pauseBackItem);
	_pauseItem->setPos(pauseBackDim.width()/2-pauseTextDim.width()/2, pauseBackDim.height()/2-pauseTextDim.height()/2);

	m_scene->addItem(_pauseBackItem);
	m_scene->addItem(_pauseItem);
}

void kMancalaBoard::play() {
	qDebug("Hide pause");
 
	if ( _pauseItem != NULL ) {
		m_scene->removeItem(_pauseItem);
		m_scene->removeItem(_pauseBackItem);
		_pauseItem = NULL;
		_pauseBackItem = NULL;
	}
}

void kMancalaBoard::hint(int id, int currentPlayer) {
	int i = ( currentPlayer == PLAYER_1 ? id : id + PLAYER_2_FIRST_GAP );

	drawHilightGap(i);
} 
