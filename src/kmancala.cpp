/* 
 * kMancala - kmancala.cpp 
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
#include <QApplication>
#include <QDate>
#include <QDebug>
#include <QtXml>

#include <kconfigdialog.h>
#include <kstatusbar.h>
#include <klocale.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardgameaction.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <kfiledialog.h>

#include "kmancala.h"
#include "kmancalamain.h"
#include "kmancalahistory.h"
#include "kmancalaprefs.h"
#include "kmancalaplayersettingsdialog.h"

#include "player.h"
#include "board.h"

#define SIZE_HEIGHT 560
#define SIZE_NO_HISTORY_WIDTH 860
#define SIZE_HISTORY_WIDTH_MIN 1024 
#define SIZE_HISTORY_WIDTH_NAX 1100
#define SIZE_MAIN_VIEW QSize(850, 475)

kMancala::kMancala()
    : KXmlGuiWindow()
{
	m_view = new kMancalaMain(SIZE_MAIN_VIEW, this);

	// tell the KXmlGuiWindow that this is indeed the main widget
	setCentralWidget(m_view);

	// then, setup our actions
	setupActions();

	// add a status bar
	statusBar()->show();
	connect(m_view, SIGNAL(statusShowMessage(QString, int)), statusBar(), SLOT(showMessage(QString, int)));

	connect(m_view, SIGNAL(gamePause()), this, SLOT(pause()));
	connect(m_view, SIGNAL(gameUnPause()), this, SLOT(play()));
	connect(m_view, SIGNAL(enableUndo()), this, SLOT(enableUndo()));
	connect(m_view, SIGNAL(disableUndo()), this, SLOT(disableUndo()));
	connect(m_view, SIGNAL(enableRedo()), this, SLOT(enableRedo()));
	connect(m_view, SIGNAL(disableRedo()), this, SLOT(disableRedo()));
	connect(m_view, SIGNAL(enableHint()), this, SLOT(enableHint()));
	connect(m_view, SIGNAL(disableHint()), this, SLOT(disableHint()));

	connect(this, SIGNAL(gamePause()), m_view, SLOT(pauseGame()));
	connect(this, SIGNAL(gameUnPause()), m_view, SLOT(unPauseGame()));

	connect(m_view, SIGNAL(gameOver(player*)), this, SLOT(gameOver(player*)));
	QString uiXMLFile = KStandardDirs::locate("appdata", "kmancalaui.rc"); 
	setupGUI(QSize(SIZE_NO_HISTORY_WIDTH, SIZE_HEIGHT), Default, uiXMLFile);

	setFixedSize(SIZE_NO_HISTORY_WIDTH, SIZE_HEIGHT);

	setSizeIncrement(10, 0);
}

kMancala::~kMancala() {}

void kMancala::setupActions() {
	KStandardGameAction::quit(this, SLOT(close()), actionCollection());
	KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
	KStandardGameAction::load(this, SLOT(openGame()), actionCollection());

	action_save = KStandardGameAction::save(this, SLOT(saveGame()), actionCollection());
	action_undo = KStandardGameAction::undo(m_view, SLOT(historyUndo()), actionCollection());
	action_redo = KStandardGameAction::redo(m_view, SLOT(historyRedo()), actionCollection());
	action_hint = KStandardGameAction::hint(m_view, SLOT(hint()), actionCollection());

	action_save->setEnabled(false);
	action_undo->setEnabled(false);
	action_redo->setEnabled(false);
	action_hint->setEnabled(false);

//	KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

	/* show history */
	action_showHistory = new KAction(i18n("Show history"), this);
	action_showHistory->setShortcut(Qt::CTRL + Qt::Key_H);
	action_showHistory->setCheckable(true);
	actionCollection()->addAction("showHistory", action_showHistory);
	connect(action_showHistory, SIGNAL(toggled(bool)), m_view, SLOT(toggleHistory(bool)));
	connect(action_showHistory, SIGNAL(toggled(bool)), this, SLOT(resize(bool)));

	/* change players */
	action_changePlayers = new KAction(i18n("Change players"), this);
	action_changePlayers->setShortcut(Qt::CTRL + Qt::Key_P);
	actionCollection()->addAction("changePlayers", action_changePlayers);
	connect(action_changePlayers, SIGNAL(triggered()), this, SLOT(changePlayers()));

	/* play pause button */
	action_playPause = new KAction(this);
	action_playPause->setText(i18n("Play/Pause"));
	action_playPause->setIcon(KIcon("media-playback-start"));
	action_playPause->setEnabled(false);
	actionCollection()->addAction("playPause", action_playPause);
	connect(action_playPause, SIGNAL(triggered()), this, SLOT(playPause()));
}

void kMancala::resize(bool checked) {
	if ( checked ) {
//		setFixedSize(SIZE_HISTORY);
		setMaximumWidth(SIZE_HISTORY_WIDTH_NAX);
		setMinimumWidth(SIZE_HISTORY_WIDTH_MIN);
	} else {
		setFixedSize(SIZE_NO_HISTORY_WIDTH, SIZE_HEIGHT);
	}
}

void kMancala::newGame() {
	bool gameWasPaused = m_view->gamePaused();
	if ( !gameWasPaused ) {
		pause();
		emit gamePause();
	}

	kMancalaPlayerSettingsDialog *dialog = new kMancalaPlayerSettingsDialog();

	player *p1 = m_view->getPlayer(PLAYER_1);
	player *p2 = m_view->getPlayer(PLAYER_2);

	if ( p1 != NULL && p2 != NULL ) {
		dialog->setPlayers(p1, p2);
	}

	if ( dialog->exec() == QDialog::Accepted ) {
		player **players = new player*[2];

		players[PLAYER_1] = dialog->getPlayers(PLAYER_1);
		players[PLAYER_2] = dialog->getPlayers(PLAYER_2);

		action_playPause->setEnabled(true);
		action_playPause->setIcon(KIcon("media-playback-pause"));
		action_playPause->setText(i18n("Pause"));
	
		action_save->setEnabled(true);

		m_view->startGame(players);
	} else {
		if ( !gameWasPaused ) {
			play();
			emit gameUnPause();
		}
	}

	delete dialog;

}

void kMancala::openGame(void) {
	bool gameWasPaused = m_view->gamePaused();
	if ( !gameWasPaused ) {
		pause();
		emit gamePause();
	}

	qDebug("Opening file");
	QString savedGamesDir = KStandardDirs::locateLocal("appdata", QString("savings/"));
	QString fileName = KFileDialog::getOpenFileName(KUrl(savedGamesDir), QString("*.xml"), this, i18n("Open Saved Game"));
	if ( !fileName.isEmpty() ) {
		qDebug("File name: %s", fileName.toStdString().c_str());

		QFile file(fileName);

		if( !file.open(QIODevice::ReadOnly) ) {
			KMessageBox::sorry(this, i18n("Cannot load the game, unable to read file"), i18n("Open Game"));
			return;
		} else {
			QDomDocument doc("kmancalagame");
			if ( !doc.setContent(&file) ) {
				KMessageBox::sorry(this, i18n("Cannot load the game, unable to parse file"), i18n("Open Game"));
				file.close();
				return;
			}
			
			QDomElement game = doc.documentElement();
			QDomElement historyElm = game.elementsByTagName("history").at(0).toElement();
			QDomElement gameElm = game.elementsByTagName("game").at(0).toElement();

			QDomNodeList moveElmList = historyElm.elementsByTagName("move");

			if ( moveElmList.count() == 0 ) {
				KMessageBox::sorry(this, i18n("Cannot load the game, no history available"), i18n("Open Game"));
				file.close();
				return; 
			}

			QList<kMancalaHistoryItem*> history;
			qDebug("Reading history ... ");
			for (int i = 0; i != moveElmList.count(); i++) {
				qDebug(" -> history item: %d", i);
				player **players = new player*[2];
				board *b = new board;

				QDomNode moveElm = moveElmList.at(i);
				int move = moveElm.attributes().namedItem("id").nodeValue().toInt();

				qDebug(" --> Reading players");
				/* players */
				QDomElement playersElm = moveElm.toElement().elementsByTagName("players").at(0).toElement();
				
				if ( playersElm.isNull() ) {
					KMessageBox::sorry(this, i18n("No players element, cannot load the game"),
								 i18n("Open Game"));
					file.close();
					return;
				}

				QDomNodeList playerElmList = playersElm.elementsByTagName("player");
				if ( playerElmList.count() != 2) {
					KMessageBox::sorry(this, i18n("No player element, cannot load the game"),
								 i18n("Open Game"));
					file.close();
					return;
				}

				for (int p = 0; p != 2; p++) {
					QDomNode playerNode = playerElmList.at(p);
					players[p] = new player(p,
						playerNode.attributes().namedItem("name").nodeValue(),
						playerNode.attributes().namedItem("type").nodeValue().toInt(),
						playerNode.attributes().namedItem("level").nodeValue().toInt());
				}

				qDebug(" --> Reading board");
				/* board */
				QDomElement boardElm = moveElm.toElement().elementsByTagName("board").at(0).toElement();
				
				if ( boardElm.isNull() ) {
					KMessageBox::sorry(this, i18n("No board element, cannot load the game"),
								 i18n("Open Game"));
					file.close();
					return;
				}

				QDomNodeList gapElmList = boardElm.elementsByTagName("gap");
				if ( gapElmList.count() != 14 ) {
					KMessageBox::sorry(this, i18n("None or too less gap elements, cannot load the game"),
								 i18n("Open Game"));
					file.close();
					return;
				}

				for (int g = 0; g != 14; g++) {
					QDomNode gapElm = gapElmList.at(g);
					int id = gapElm.attributes().namedItem("id").nodeValue().toInt();
					int value = gapElm.toElement().text().toInt();
					b->setFieldBeans(id, value);
				}

				qDebug(" --> Reading playerLastMove");
				/* player last move */
				QDomElement playerLastMoveElm = moveElm.toElement().elementsByTagName("playerLastMove").at(0).toElement();
				if ( playerLastMoveElm.isNull() ) {
					KMessageBox::sorry(this, i18n("No playerLastMove elemenet, cannot load the game"),
								 i18n("Open Game"));
					file.close();
					return;
				}

				int playerLastTurn = playerLastMoveElm.toElement().text().toInt();

				if ( playerLastTurn != PLAYER_1 && playerLastTurn != PLAYER_2 ) {
					KMessageBox::sorry(this, i18n("Incorrect value in playerLastTurn, cannot load the game"),
								 i18n("Open Game"));
					qDebug("Player Last Turn: %d [allowed values: %d, %d]", playerLastTurn, PLAYER_1, PLAYER_2);
					file.close();
					return;
				}


				qDebug(" --> Reading playerOnTurn");
				/* player on move */
				QDomElement playerOnMoveElm = moveElm.toElement().elementsByTagName("playerOnMove").at(0).toElement();

				if ( playerOnMoveElm.isNull() ) {
					KMessageBox::sorry(this, i18n("No playerOnTurn element, cannot load the game"),
								 i18n("Open Game"));
					file.close();
					return;
				}

				int playerOnTurn = playerOnMoveElm.toElement().text().toInt();

				if ( playerOnTurn != PLAYER_1 && playerOnTurn != PLAYER_2 ) {
					KMessageBox::sorry(this, i18n("Incorrect value in playerOnTurn, cannot load the game"),
								 i18n("Open Game"));
					file.close();
					return;
				}

				QDebug dbg(qDebug()); 

				dbg << " ----> Player 1: " << players[0]->getName() << ", Type: " << players[0]->getType() << ", Level: " << players[0]->getLevel();
				dbg << "\n";
				dbg << " ----> Player 2: " << players[1]->getName() << ", Type: " << players[1]->getType() << ", Level: " << players[1]->getLevel();

				dbg << " ----> Board: "; 
				for (int abc = 0; abc != 14; abc++) {
					dbg << " ----> Field["<<abc<<"] " << b->getFieldBeans(abc) << "\n";
				}

				dbg << " ----> Player Last Turn: " << playerLastTurn;
				dbg << "\n";
				dbg << " ----> Player On Turn: " << playerOnTurn;
				dbg << "\n";
				dbg << " ----> Move: " << move;

				history.push_back(new kMancalaHistoryItem(NULL, b, players, playerLastTurn, playerOnTurn, move));
			}

			action_playPause->setEnabled(true);
			action_playPause->setIcon(KIcon("media-playback-pause"));
			action_playPause->setText(i18n("Pause"));

			action_save->setEnabled(true);

			m_view->loadGame(history);
		}
	} else {
		/* pause only in case cancel button is pressed, otherwise do not unpause loaded game */
		if ( !gameWasPaused ) {
			play();
			emit gameUnPause();
		}
	}
}

void kMancala::saveGame(void) {
	bool gameWasPaused = m_view->gamePaused();

	if ( !gameWasPaused ) {
		pause();
		emit gamePause();
	}

	QDate today = QDate::currentDate();
	QString file = today.toString(QString("dd-MM-yyyy"));

	QString saveGameDefaultFile = KStandardDirs::locateLocal("appdata", QString("savings/") + file + ".xml");
	QString fileName = KFileDialog::getSaveFileName(KUrl(saveGameDefaultFile), QString("*.xml"), this, i18n("Save Game"));

	if ( !fileName.isEmpty() ) {
		GAME_STATE *gameState = m_view->getGameState();

		QDomDocument game("kMancalaGame");
		QDomElement root = game.createElement("kmancalagame");
		game.appendChild(root);

		QDomElement history = game.createElement("history");
		root.appendChild(history);

		while ( !gameState->history.isEmpty() ) {
			kMancalaHistoryItem *i = gameState->history.takeFirst();
			QDomElement playersEl = game.createElement("players");

			player **p = i->getPlayers();
			for (int i = 0; i != 2; i++) {	
				QDomElement player = game.createElement("player");
				player.setAttribute("name", p[i]->getName());	
				player.setAttribute("type", p[i]->getType());	
				player.setAttribute("level", p[i]->getLevel());
				playersEl.appendChild(player);
			}

			QDomElement moveEl = game.createElement("move");
			QDomElement boardEl = game.createElement("board");
			moveEl.setAttribute("id", i->getMove());
			moveEl.appendChild(playersEl);
			moveEl.appendChild(boardEl);

			board *b = i->getBoard();

			for (int f = 0; f != 14; f++) {
				QDomText value = game.createTextNode(QString().setNum(b->getFieldBeans(f)));
				QDomElement gap = game.createElement("gap");
				gap.setAttribute("id", f);
				gap.appendChild(value);
				boardEl.appendChild(gap);
			}

			int pLastMove = i->getPlayerLastTurn();
			QDomElement playerLastMove = game.createElement("playerLastMove");
			playerLastMove.appendChild(game.createTextNode(QString().setNum(pLastMove)));
			moveEl.appendChild(playerLastMove);

			int pOnMove = i->getPlayerOnTurn();
			QDomElement playerOnMove = game.createElement("playerOnMove");
			playerOnMove.appendChild(game.createTextNode(QString().setNum(pOnMove)));
			moveEl.appendChild(playerOnMove);

			history.appendChild(moveEl);
		}

		QDomElement gameStatus = game.createElement("game");
		gameStatus.appendChild(game.createTextNode(QString().setNum(gameState->game)));
		root.appendChild(gameStatus);
 
		QFile file(fileName);
		if( !file.open(QIODevice::WriteOnly) ) {
			KMessageBox::sorry(this, i18n("Cannot save game, unable to write to file"), i18n("Save Game"));
		} else {
			QTextStream ts(&file);
			ts << game.toString();

			statusBar()->showMessage("Game Saved", 3000);
			file.close();
		}
	}

	if ( !gameWasPaused ) {
		play();
		emit gameUnPause();
	}
}

void kMancala::changePlayers(void) {
	if ( m_view->gameActive() ) {
		kMancalaPlayerSettingsDialog *dialog = new kMancalaPlayerSettingsDialog();
		dialog->setTitle(i18n("Change players"));

		player *p1 = m_view->getPlayer(PLAYER_1);
		player *p2 = m_view->getPlayer(PLAYER_2);

		if ( p1 != NULL && p2 != NULL ) {
			dialog->setPlayers(p1, p2);
		}
		
		bool gameWasPaused = m_view->gamePaused();

		if ( !gameWasPaused ) {
			pause();
			emit gamePause();
		}

		if ( dialog->exec() == QDialog::Accepted ) {
			player **players = new player*[2];

			players[PLAYER_1] = dialog->getPlayers(PLAYER_1);
			players[PLAYER_2] = dialog->getPlayers(PLAYER_2);

			qDebug() << "Player 1: " << players[PLAYER_1]->getName() << " Type: " << players[PLAYER_1]->getType() << " Level: " << players[PLAYER_1]->getLevel();
			qDebug() << "Player 2: " << players[PLAYER_2]->getName() << " Type: " << players[PLAYER_2]->getType() << " Level: " << players[PLAYER_2]->getLevel();

			m_view->updatePlayers(players);
		}

		if ( !gameWasPaused ) {
			play();
			emit gameUnPause();
		}

		delete dialog;
	} else {
		KMessageBox::sorry(this, i18n("You have to start a game first"), i18n("No active game"));
	}
}

void kMancala::play(void) {
	qDebug("Playing ...");
	action_playPause->setIcon(KIcon("media-playback-pause"));
	action_playPause->setText(i18n("Pause"));
	action_playPause->setEnabled(true);

}

void kMancala::pause(void) {
	qDebug("Pausing ...");
	action_playPause->setIcon(KIcon("media-playback-start"));
	action_playPause->setText(i18n("Play"));
	action_playPause->setEnabled(true);
}

void kMancala::playPause(void) {
	qDebug("playPause");
	if ( m_view->gamePaused() ) {
		qDebug("gamePaused");
		emit gameUnPause();
		play();
	} else {
		qDebug("game Not Paused");
		emit gamePause();
		pause();
	}
}

void kMancala::gameOver(player *p) {
	qDebug("kmancala.cpp: Gave over received, disabled play/pause button");
	action_playPause->setText(i18n("Play/Pause"));
	action_playPause->setIcon(KIcon("media-playback-start"));
	action_playPause->setEnabled(false);
}

void kMancala::enableHint() {
	action_hint->setEnabled(true);
}

void kMancala::disableHint() {
	action_hint->setEnabled(false);
}

void kMancala::enableRedo() {
	action_redo->setEnabled(true);
}

void kMancala::disableRedo() {
	action_redo->setEnabled(false);
}

void kMancala::enableUndo() {
	action_undo->setEnabled(true);
}

void kMancala::disableUndo() {
	action_undo->setEnabled(false);
}

void kMancala::closeEvent(QCloseEvent *event) {

	if ( m_view->gameActive() ) {
		bool gameWasPaused = m_view->gamePaused();

		if ( !gameWasPaused ) {
			pause();
			emit gamePause();
		}

		if ( KMessageBox::questionYesNo(this, 
					i18n("There is a game in progress. Do you really want to quit right now?"), 
					i18n("Quit game")) == KMessageBox::Yes ) {
			QMainWindow::closeEvent(event);
		} else {
			event->ignore();
		}

		if ( !gameWasPaused ) {
			play();
			emit gameUnPause();
		}
	}
}

void kMancala::optionsPreferences() {
    // The preference dialog is derived from prefs_base.ui
    //
    // compare the names of the widgets in the .ui file
    // to the names of the variables in the .kcfg file
    //avoid to have 2 dialogs shown
    if ( KConfigDialog::showDialog( "settings" ) )  {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, "settings", kMancalaPrefs::self());
    QWidget *generalSettingsDlg = new QWidget;

    dialog->addPage(generalSettingsDlg, i18n("General"), "package_setting");

    connect(dialog, SIGNAL(settingsChanged(QString)), m_view, SLOT(settingsChanged()));

    dialog->setAttribute( Qt::WA_DeleteOnClose );
    dialog->show();
}
