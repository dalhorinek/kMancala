/* 
 * kMancala - main.cpp 
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
#include <stdio.h>

#include "main.h"
#include "kmancala.h"

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

int main(int argc, char **argv)
{

	char copyright[255]; 
	sprintf(copyright, "(c) %d %s", PROGRAM_COPYRIGHT_YEAR, PROGRAM_AUTHOR);

	KAboutData about("kmancala",
				0, 
				ki18n(PROGRAM_NAME),
				PROGRAM_VERSION,
				ki18n("Kalah aka Mancala is a count-and-capture board game"), 
				KAboutData::License_GPL,
				ki18n(copyright),
				KLocalizedString(), 
				PROGRAM_HOMEPAGE,
				PROGRAM_AUTHOR_EMAIL
	);

	about.addAuthor( ki18n(PROGRAM_AUTHOR), KLocalizedString(), PROGRAM_AUTHOR_EMAIL);
	KCmdLineArgs::init(argc, argv, &about);

	KCmdLineOptions options;
	options.add("+[URL]", ki18n("Game to open" ));
	KCmdLineArgs::addCmdLineOptions(options);
	KApplication app;

	kMancala *widget = new kMancala;
	widget->show();

	return app.exec();
}
