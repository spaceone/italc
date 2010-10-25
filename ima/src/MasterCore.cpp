/*
 * MasterCore.cpp - management of application-global instances
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

#include <italcconfig.h>

#include <QtNetwork/QHostAddress>

#include "MasterCore.h"
#include "ItalcVncConnection.h"
#include "ItalcCoreConnection.h"
#include "ClassroomManager.h"
#include "GlobalConfig.h"
#include "PersonalConfig.h"
#include "DemoServerMaster.h"


namespace MasterCore
{

// static data initialization
ItalcVncConnection * localDisplay = NULL;
int localDisplayPort = PortOffsetIVS;

GlobalConfig * globalConfig = NULL;
PersonalConfig * personalConfig = NULL;

ItalcSlaveManager *italcSlaveManager;

ItalcCoreConnection * localCore = NULL;

ClassroomManager * classroomManager = NULL;

DemoServerMaster * demoServerMaster = NULL;


void init( void )
{
	localDisplay = new ItalcVncConnection;
	localDisplay->setHost( QHostAddress(
#ifdef ITALC_IPV6_SUPPORT
					QHostAddress::LocalHostIPv6
#else
					QHostAddress::LocalHost
#endif
								).toString() );
	if( localDisplayPort != PortOffsetIVS )
	{
		localDisplay->setPort( localDisplayPort );
	}
	localDisplay->setFramebufferUpdateInterval( -1 );
	localDisplay->start();
	if( !localDisplay->waitForConnected( 5000 ) )
	{
		// TODO
	}

	globalConfig = new GlobalConfig( Configuration::Store::XmlFile );
	personalConfig = new PersonalConfig( Configuration::Store::XmlFile );

	localCore = new ItalcCoreConnection( localDisplay );
	localCore->setRole( ItalcCore::role );

	italcSlaveManager = new ItalcSlaveManager;
	italcSlaveManager->demoServerMaster()->
								start( PortOffsetIVS, PortOffsetDemoServer );

	classroomManager = new ClassroomManager;


}

void deinit( void )
{
	personalConfig->flushStore();
	globalConfig->flushStore();

	delete classroomManager;
	delete localCore;
	delete personalConfig;
	delete globalConfig;
	delete localDisplay;
}


}


