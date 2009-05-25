/*
 * Classroom.h - definition of class Classroom
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


#ifndef _CLASSROOM_H
#define _CLASSROOM_H

#include <QtCore/QString>

#include "Client.h"

class ClassroomManager;


class Classroom
{
public:
	Classroom( const QString & _name = QString(),
				ClassroomManager * _classroom_manager = NULL );
	virtual ~Classroom();

	inline const QString & name( void ) const
	{
		return m_name;
	}

	void addClient( Client * _c )
	{
		m_clients << _c;
	}

	void removeClient( int _idx )
	{
		m_clients.removeAt( _idx );
	}

	Client * clientAt( int _idx )
	{
		return m_clients[_idx];
	}

	const ClientList & clients( void ) const
	{
		return m_clients;
	}

	inline bool isActive( void ) const
	{
		return m_active;
	}

	inline void setActive( bool _active )
	{
		m_active = _active;
	}


private:
	ClassroomManager * m_classroomManager;
	QString m_name;
	bool m_active;

	ClientList m_clients;

} ;

typedef QList<Classroom *> ClassroomList;


#endif
