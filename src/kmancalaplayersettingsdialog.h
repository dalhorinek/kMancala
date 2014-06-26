/* 
 * kMancala - kmancalaplayersettingsdialog.h
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
#ifndef KMANCALANEWGAMEDIALOG_H
#define KMANCALANEWGAMEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QSignalMapper>

#include "player.h"

class kMancalaPlayerSettingsDialog : public QDialog
{
Q_OBJECT

private:
	QVBoxLayout *mainLayout;

	QHBoxLayout *player_0_layoutName;
	QLineEdit *player_0_layoutNameInput;
	QHBoxLayout *player_0_layoutType;
	QComboBox *player_0_layoutTypeBox;
	QHBoxLayout *player_0_layoutLevel;
	QComboBox *player_0_layoutLevelBox;

	QHBoxLayout *player_1_layoutName;
	QLineEdit *player_1_layoutNameInput;
	QHBoxLayout *player_1_layoutType;
	QComboBox *player_1_layoutTypeBox;
	QHBoxLayout *player_1_layoutLevel;
	QComboBox *player_1_layoutLevelBox;

	int _getLevelIndex(int level);
public:
	kMancalaPlayerSettingsDialog();
	player *getPlayers(int id);
	void setPlayers(player *one, player *two);
	void setTitle(QString title);

public slots:
	void onTypeSelectPlayer0(int index);
	void onTypeSelectPlayer1(int index);
};

#endif // KMANCALANEWGAMEDIALOG_H
