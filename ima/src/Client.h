/*
 * Client.h - declaration of class Client which represents a client, shows its
 *            display and allows controlling it in several ways
 *
 * Copyright (c) 2004-2010 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
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


#ifndef _CLIENT_H
#define _CLIENT_H

#include <italcconfig.h>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QPoint>


class ItalcCoreConnection;
class ItalcVncConnection;


class Client : public QObject
{
	Q_OBJECT
public:
	enum Types
	{
		Type_Student,
		Type_Teacher,
		Type_Other
	} ;
	typedef Types Type;

	enum Modes
	{
		Mode_Overview,
		Mode_FullscreenDemo,
		Mode_WindowDemo,
		Mode_Locked,
		Mode_Unknown
	} ;
	typedef Modes Mode;

	enum States
	{
		State_Inactive,
		State_Unreachable,
		State_NoUserLoggedIn,
		State_Operating,
		State_Unkown
	} ;
	typedef States State;

	Client( const QString & _host,
		const QString & _mac,
		const QString & _displayName,
		Type _type );

	virtual ~Client();

	inline Type type() const
	{
		return m_type;
	}

	inline void setType( Types _type )
	{
		if( _type >= Type_Student && _type <= Type_Other )
		{
			m_type = _type;
		}
	}

	inline Mode mode() const
	{
		return m_mode;
	}

	inline State state() const
	{
		return m_state;
	}

	inline QString displayName() const
	{
		return m_displayName.isEmpty() ? m_host : m_displayName;
	}

	inline const QString & host() const
	{
		return m_host;
	}

	inline const QString & mac() const
	{
		return m_mac;
	}

	inline const QString & user() const
	{
		return m_user;
	}

	inline bool isSelected() const
	{
		return m_selected;
	}

	inline void setSelected( bool _s )
	{
		m_selected = _s;
	}

	inline void setDisplayName( const QString & _displayName )
	{
		m_displayName = _displayName;
	}

	inline void setHost( const QString & _host )
	{
		m_host = _host;
	}

	inline void setMac( const QString & _mac )
	{
		m_mac = _mac;
	}

	inline const QPoint & rasterPosition() const
	{
		return m_rasterPosition;
	}

	void openConnection();
	void closeConnection();

	void update()
	{
		emit updated();
	}

	// action-handlers
	void changeMode( const Mode _new_mode );
	void viewLive();
	void remoteControl();
	void clientDemo();
	void displayTextMessage( const QString & _msg );
	void logonUser( const QString & _username, const QString & _password,
			const QString & _domain );
	void logoutUser();
	void snapshot();
	void powerOn();
	void reboot();
	void powerDown();
	void execCmds( const QString & _cmds );


private:
	States currentState() const;


	Types m_type;
	Modes m_mode;
	States m_state;

	ItalcVncConnection * m_vncConn;
	ItalcCoreConnection * m_coreConn;

	QString m_displayName;
	QString m_host;
	QString m_mac;
	QString m_user;

	bool m_selected;

	bool m_makeSnapshot;

	QPoint m_rasterPosition;


signals:
	void updated();

} ;


typedef QList<Client *> ClientList;


#endif
