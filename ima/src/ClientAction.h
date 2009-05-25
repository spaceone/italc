/*
 * ClientAction.h - class ClientAction, a proxy for actions on clients
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


#ifndef _CLIENT_ACTION_H
#define _CLIENT_ACTION_H

#include <QtGui/QAction>

#include "Client.h"
#include "ItalcCore.h"


class ClientAction : public QAction
{
	Q_OBJECT
public:
	enum TargetGroup
	{
		Default,
		SelectedClients,
		VisibleClients
	} ;

	enum Flags
	{
		None = 0,
		FullMenu = 1
	} ;

	ClientAction( const ItalcCore::Command & _cmd, QObject * _parent = 0,
							int _flags = 0 );
	ClientAction( const ItalcCore::Command & _cmd, const QIcon & _icon,
			const QString & _text, QObject * _parent = 0,
							int _flags = 0 );
	virtual ~ClientAction();

	void process( const ClientList & _clients,
				TargetGroup _target = Default );
	static void process( QAction * _action,
		const ClientList & _clients, TargetGroup _target = Default );

	inline bool flags( int _mask = -1 )
	{
		return ( m_flags & _mask );
	}


private:
	ItalcCore::Command m_cmd;
	int m_flags;

	bool confirmLogout( TargetGroup _target ) const;
	bool confirmReboot( TargetGroup _target ) const;
	bool confirmPowerDown( TargetGroup _target ) const;
	QString dataExpanded( const ClientList & _clients ) const;

} ;


#endif
