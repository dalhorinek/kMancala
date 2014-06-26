/* 
 * kMancala - kmancala.h
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
#ifndef KMANCALA_H
#define KMANCALA_H

#include "player.h"

#include <QCloseEvent>
#include <KXmlGuiWindow>
#include <KAction>

class kMancalaMain;

/**
 * This class serves as the main window for kMancala.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */
class kMancala : public KXmlGuiWindow
{
Q_OBJECT
public:
	kMancala();
	~kMancala();
	void closeEvent(QCloseEvent *event);

private slots:
	void newGame();
	void openGame();
	void saveGame();
	void optionsPreferences(); 
	void resize(bool checked);
	void changePlayers();
	void playPause();
	void play();
	void pause();
	void gameOver(player *p);
	void enableHint();
	void disableHint();
	void enableRedo();
	void disableRedo();
	void enableUndo();
	void disableUndo();
	

private:
	void setupActions();
	kMancalaMain *m_view;

	KAction *action_showHistory;
	KAction *action_changePlayers;
	KAction *action_playPause;
	KAction *action_save;
	KAction *action_hint;
	KAction *action_undo;
	KAction *action_redo;

signals:
	void gamePause();
	void gameUnPause();

};

#endif // _KMANCALA_H_
