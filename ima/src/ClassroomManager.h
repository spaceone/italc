/*
 * ClassroomManager.h - declaration of ClassroomManager
 *
 * Copyright (c) 2004-2009 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
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


#ifndef _CLASSROOM_MANAGER_H
#define _CLASSROOM_MANAGER_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include "Classroom.h"



class ClassroomManager : public QObject
{
	Q_OBJECT
public:
	ClassroomManager();
	virtual ~ClassroomManager();


	ClientList activeClients( void ) const;

	const ClassroomList & classrooms( void ) const
	{
		return m_classrooms;
	}

	Client::Modes globalClientMode( void ) const
	{
		return m_globalClientMode;
	}


public slots:
	void updateClients( void );

	// slots for toolbar-actions
	void changeGlobalClientMode( int );
	void sendMessage( void );
	void powerOnClients( void );
	void powerDownClients( void );
	void remoteLogon( void );
	void directSupport( void );

	// slots for actions in view-menu
	void adjustWindows( void );
	void arrangeWindowsToggle( bool _on );
	void increaseClientSize( void );
	void decreaseClientSize( void );

	// slots for config-widget in side-bar
	void updateIntervalChanged( int _value );


private:
	ClassroomList m_classrooms;

	Client::Modes m_globalClientMode;

} ;


#endif
