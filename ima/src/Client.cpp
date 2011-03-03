/*
 * Client.cpp - implementation of class Client which represents data and
 *              functionality of a client
 *
 * Copyright (c) 2004-2011 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
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

#include "Client.h"
#include "ItalcVncConnection.h"
#include "ItalcConfiguration.h"
#include "ItalcCoreConnection.h"
#include "MasterCore.h"
#include "PersonalConfig.h"
#include "ClassroomManager.h"
#include "MainWindow.h"
#include "MasterUI.h"




Client::Client( const QString & _host,
		const QString & _mac,
		const QString & _displayName,
		Type _type ) :
	m_type( _type ),
	m_mode( Mode_Overview ),
	m_state( State_Inactive ),
	m_vncConn( new ItalcVncConnection ),
	m_coreConn( new ItalcCoreConnection( m_vncConn ) ),
	m_framebufferUpdated( false ),
	m_displayName( _displayName ),
	m_host( _host ),
	m_mac( _mac ),
	m_user(),
	m_selected( false ),
	m_makeSnapshot( false ),
	m_rasterPosition( -1, -1 )
{
	m_vncConn->setHost( m_host );
	m_vncConn->setQuality( ItalcVncConnection::ThumbnailQuality );
	m_vncConn->setFramebufferUpdateInterval(
				MasterCore::personalConfig->clientUpdateInterval() * 1000 );
}




Client::~Client()
{
	closeConnection();

	delete m_coreConn;
	m_coreConn = NULL;

	delete m_vncConn;
	m_vncConn = NULL;
}




void Client::openConnection()
{
	m_vncConn->start();
}




void Client::closeConnection()
{
	changeMode( Mode_Overview );
	m_vncConn->stop();
}




void Client::changeMode( const Modes _new_mode )
{
	if( _new_mode != m_mode )
	{
		// properly leave old mode
		switch( m_mode )
		{
			case Mode_Overview:
			case Mode_Unknown:
				break;
			case Mode_FullscreenDemo:
			case Mode_WindowDemo:
				MasterCore::localCoreService->demoServerUnallowHost( m_host );
				m_coreConn->stopDemo();
				break;
			case Mode_Locked:
				m_coreConn->unlockScreen();
				break;
		}

		// switch into new mode
		switch( m_mode = _new_mode )
		{
			case Mode_Overview:
			case Mode_Unknown:
				break;
			case Mode_FullscreenDemo:
			case Mode_WindowDemo:
				MasterCore::localCoreService->demoServerAllowHost( m_host );
				m_coreConn->startDemo(
								QString(),// let client guess IP from connection
								ItalcCore::config->demoServerPort(),
								m_mode == Mode_FullscreenDemo );
				break;
			case Mode_Locked:
				m_coreConn->lockScreen();
				break;
		}
	}
	// if connection was lost while sending commands such as stop-demo,
	// there should be a way for switching back into normal mode
	else if( m_mode == Mode_Overview )
	{
		m_coreConn->stopDemo();
		m_coreConn->unlockScreen();
	}
}





/*void Client::reload()
{
	if( m_coreConn->isConnected() )
	{
		m_coreConn->sendGetUserInformationRequest();
		m_user = m_connection->user();
	}
	else
	{
		m_vncConn->reset( m_host );
		m_user = QString();
	}
}*/




void Client::setUpdateFlag()
{
	m_framebufferUpdated = true;
}




void Client::clientDemo()
{
	ClassroomManager * cm = MasterCore::classroomManager;
	cm->changeGlobalClientMode( Mode_Overview );

	QList<Client *> vc = cm->activeClients();

	foreach( Client * cl, vc )
	{
		if( cl != this )
		{
			cl->changeMode( Mode_FullscreenDemo );
		}
	}

	//MasterUI::mainWindow->checkModeButton( Client::Mode_FullscreenDemo );

	MasterUI::mainWindow->remoteControlDisplay( m_host, true, true );
}




void Client::viewLive()
{
	changeMode( Mode_Overview );

	MasterUI::mainWindow->remoteControlDisplay( m_host, true );
}




void Client::remoteControl()
{
	changeMode( Mode_Overview );

	MasterUI::mainWindow->remoteControlDisplay( m_host );
}




void Client::displayTextMessage( const QString & _msg )
{
	m_coreConn->displayTextMessage( _msg );
}




/*void Client::logonUser( const QString & _username, const QString & _password,
			const QString & _domain )
{
	m_coreConn->logonUser( _username, _password, _domain );
}




void Client::logoutUser()
{
	m_coreConn->logoutUser();
}




void Client::snapshot()
{
	m_takeSnapshot = true;
}





void Client::powerOn()
{
	// we have to send the wake-on-LAN packets with root privileges,
	// therefore let the local ICA do the job (as it usually is running
	// with higher privileges)
	MasterCore::localCoreService->powerOnComputer( m_mac );
}




void Client::reboot()
{
	m_coreConn->restartComputer();
}





void Client::powerDown()
{
	m_coreConn->powerDownComputer();
}




void Client::execCmds( const QString & _cmds )
{
	m_coreConn->execCmds( _cmds );
}




Client::States Client::currentState() const
{
	if( m_vncConn->isRunning() )
	{
		if( m_vncConn->isConnected() )
		{
			return State_Operating;
		}
		return State_Unreachable;
	}
	return State_Inactive;
}


