/* 
 * kMancala - kmancalaplayersettingsdialog.cpp 
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
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>

#include <klocale.h>

#include "kmancalaplayersettingsdialog.h"
#include "player.h"

kMancalaPlayerSettingsDialog::kMancalaPlayerSettingsDialog()
    : QDialog()
{
	mainLayout = new QVBoxLayout();

	QVBoxLayout *player_settingsBox[2];
	player_settingsBox[0] = new QVBoxLayout;
	player_settingsBox[0]->addWidget(new QLabel(i18n("<h3>Player 1</h3>")));

	QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	/* player 1 */
	player_0_layoutName = new QHBoxLayout;
	player_0_layoutName->addWidget(new QLabel(i18n("Name")));
	player_0_layoutNameInput = new QLineEdit;
	player_0_layoutNameInput->setText(i18n("Player 1"));
	player_0_layoutName->addWidget(player_0_layoutNameInput);

	player_0_layoutType = new QHBoxLayout;
	player_0_layoutTypeBox = new QComboBox;
	player_0_layoutTypeBox->addItem(i18n("Human"), QVariant(PLAYER_HUMAN));
	player_0_layoutTypeBox->addItem(i18n("Computer"), QVariant(PLAYER_AI));

	player_0_layoutType->addWidget(new QLabel(i18n("Type")));
	player_0_layoutType->addWidget(player_0_layoutTypeBox);

	player_0_layoutLevel = new QHBoxLayout;
	player_0_layoutLevelBox = new QComboBox;
	player_0_layoutLevelBox->addItem(QString(i18n("Piece of Cake")), QVariant(1));
	player_0_layoutLevelBox->addItem(QString(i18n("Gimme Some")), QVariant(2));
	player_0_layoutLevelBox->addItem(QString(i18n("Let's Rock")), QVariant(3));
	player_0_layoutLevelBox->addItem(QString(i18n("Nightmare")), QVariant(4));

	player_0_layoutLevel->addWidget(new QLabel(i18n("Computer Level")));
	player_0_layoutLevel->addWidget(player_0_layoutLevelBox);

	player_settingsBox[0]->addLayout(player_0_layoutName);
	player_settingsBox[0]->addLayout(player_0_layoutType);
	player_settingsBox[0]->addLayout(player_0_layoutLevel);
	player_0_layoutLevelBox->setDisabled(true);

	player_settingsBox[1] = new QVBoxLayout;
	player_settingsBox[1]->addWidget(new QLabel(i18n("<h3>Player 2</h3>")));

	player_1_layoutName = new QHBoxLayout;
	player_1_layoutName->addWidget(new QLabel(i18n("Name")));
	player_1_layoutNameInput = new QLineEdit;
	player_1_layoutNameInput->setText(i18n("Player 2"));
	player_1_layoutName->addWidget(player_1_layoutNameInput);

	player_1_layoutType = new QHBoxLayout;
	player_1_layoutTypeBox = new QComboBox;
	player_1_layoutTypeBox->addItem(i18n("Human"), QVariant(PLAYER_HUMAN));
	player_1_layoutTypeBox->addItem(i18n("Computer"), QVariant(PLAYER_AI));

	player_1_layoutType->addWidget(new QLabel(i18n("Type")));
	player_1_layoutType->addWidget(player_1_layoutTypeBox);

	player_1_layoutLevel = new QHBoxLayout;
	player_1_layoutLevelBox = new QComboBox;
	player_1_layoutLevelBox->addItem(QString(i18n("Piece of Cake")), QVariant(1));
	player_1_layoutLevelBox->addItem(QString(i18n("Gimme Some")), QVariant(2));
	player_1_layoutLevelBox->addItem(QString(i18n("Let's Rock")), QVariant(3));
	player_1_layoutLevelBox->addItem(QString(i18n("Nightmare")), QVariant(4));

	player_1_layoutLevel->addWidget(new QLabel(i18n("Computer Level")));
	player_1_layoutLevel->addWidget(player_1_layoutLevelBox);

	player_settingsBox[1]->addLayout(player_1_layoutName);
	player_settingsBox[1]->addLayout(player_1_layoutType);
	player_settingsBox[1]->addLayout(player_1_layoutLevel);
	player_1_layoutLevelBox->setDisabled(true);

	connect(player_0_layoutTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeSelectPlayer0(int)));
	connect(player_1_layoutTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeSelectPlayer1(int)));

	mainLayout->addLayout(player_settingsBox[0]);
	mainLayout->addLayout(player_settingsBox[1]);
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);
	setWindowTitle(i18n("New Game"));
	setModal(true);
	setFixedSize(250, 250);
}

void kMancalaPlayerSettingsDialog::setTitle(QString title) {
	setWindowTitle(title);
}

int kMancalaPlayerSettingsDialog::_getLevelIndex(int level) {
	qDebug() << "Level: " << level;

	for (int i = 0; i != player_0_layoutLevelBox->count(); i++) {
		QVariant d = player_0_layoutLevelBox->itemData(i);

		qDebug() << "Candidate: " << d.toInt();

		if ( d.toInt() == level ) {
			qDebug() << "Id of level: " << level << " i: " << i;
			return i;
		}
	} 

	return 0;
}

void kMancalaPlayerSettingsDialog::setPlayers(player *one, player *two) {
	player_0_layoutNameInput->setText(one->getName());
	int p0_type = one->getType(); 
	player_0_layoutTypeBox->setCurrentIndex(p0_type-1);

	if ( p0_type == PLAYER_AI )
		player_0_layoutLevelBox->setCurrentIndex(_getLevelIndex(one->getLevel()));

	player_1_layoutNameInput->setText(two->getName());
	int p1_type = two->getType();
	player_1_layoutTypeBox->setCurrentIndex(p1_type-1);
	if ( p1_type == PLAYER_AI )
		player_1_layoutLevelBox->setCurrentIndex(_getLevelIndex(two->getLevel()));
}

player* kMancalaPlayerSettingsDialog::getPlayers(int id) {

	player *result = NULL; 
	if ( id == PLAYER_1 ) {
		result = new player(PLAYER_1, player_0_layoutNameInput->text(), player_0_layoutTypeBox->itemData(player_0_layoutTypeBox->currentIndex()).toInt());

		if ( result->getType() == PLAYER_AI ) {
			result->setLevel(player_0_layoutLevelBox->itemData(player_0_layoutLevelBox->currentIndex()).toInt());
		}
	} else if ( id == PLAYER_2 ) {
		result = new player(PLAYER_2, player_1_layoutNameInput->text(), player_1_layoutTypeBox->itemData(player_1_layoutTypeBox->currentIndex()).toInt());

		if ( result->getType() == PLAYER_AI ) {
			result->setLevel(player_1_layoutLevelBox->itemData(player_1_layoutLevelBox->currentIndex()).toInt());
		}
	}

	return result;
}

void kMancalaPlayerSettingsDialog::onTypeSelectPlayer0(int index)
{
    QVariant typeSelected = player_0_layoutTypeBox->itemData(index);
    int type = typeSelected.toInt();
    
    if ( type == 1 ) {
	player_0_layoutLevelBox->setDisabled(true);
    } else if ( type == 2 ) { 
	player_0_layoutLevelBox->setDisabled(false);
    }
}

void kMancalaPlayerSettingsDialog::onTypeSelectPlayer1(int index)
{
    QVariant typeSelected = player_1_layoutTypeBox->itemData(index);
    int type = typeSelected.toInt();
    
    if ( type == 1 ) {
	player_1_layoutLevelBox->setDisabled(true);
    } else if ( type == 2 ) { 
	player_1_layoutLevelBox->setDisabled(false);
    }
}
