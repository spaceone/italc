/*
 * MasterCore.h - namespace for all global instances of various objects
 *
 * Copyright (c) 2008-2009 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
 *
 * This file is part of iTALC - http://italc.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */


#ifndef _MASTER_CORE_H
#define _MASTER_CORE_H

class ItalcVncConnection;
class ItalcCoreConnection;
class ItalcSlaveManager;
class ClassroomManager;
class GlobalConfig;
class PersonalConfig;
class MainWindow;


namespace MasterCore
{
	void init( void );
	void deinit( void );

	extern ItalcVncConnection * localDisplay;
	extern int localDisplayPort;

	extern GlobalConfig * globalConfig;
	extern PersonalConfig * personalConfig;

	extern ItalcSlaveManager *italcSlaveManager;

	extern ItalcCoreConnection * localCoreService;

	extern ClassroomManager * classroomManager;

	// UI objects
	extern MainWindow * mainWindow;

} ;


#endif
