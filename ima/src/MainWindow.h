/*
 * MainWindow.h - main-window of iTALC
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

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <QtCore/QReadWriteLock>
#include <QtGui/QButtonGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QToolButton>

#include "ui_MainWindow.h"

#include "Client.h"
#include "SnapshotList.h"


class QGraphicsScene;
class QSplashScreen;
class QSplitter;
class ItalcCoreConnection;
class ClassroomManager;
class ConfigWidget;
class OverviewWidget;
class RemoteControlWidget;
class UserList;



class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
public:
	MainWindow( int _screen );
	virtual ~MainWindow();

	QGraphicsScene * workspace()
	{
		return m_workspace;
	}


	void checkModeButton( int _id )
	{
		QToolButton * btn = dynamic_cast<QToolButton *>(
						m_modeGroup->button( _id ) );
		if( btn != NULL )
		{
			btn->setChecked( true );
		}
	}

	inline MainToolBar * toolBar()
	{
		return m_toolBar;
	}

	inline SideBar * sideBar()
	{
		return m_sideBar;
	}

	static bool ensureConfigPathExists();

	void remoteControlDisplay( const QString & _hostname,
					bool _view_only = false,
					bool _stop_demo_afterwards = false );

	inline bool remoteControlRunning()
	{
		QReadLocker rl( &m_rctrlLock );
		return m_remoteControlWidget != NULL;
	}

	inline void reloadSnapshotList()
	{
		m_snapshotList->reloadList();
	}


protected:
	void keyPressEvent( QKeyEvent *e );


private slots:
	void handleSystemTrayEvent( QSystemTrayIcon::ActivationReason _r );
	void remoteControlClient( QAction * _c );
	void remoteControlWidgetClosed( QObject * );

	void aboutITALC();

	void changeGlobalClientMode( int );

	void mapOverview()
	{
		changeGlobalClientMode( Client::Mode_Overview );
	}
	void mapFullscreenDemo()
	{
		changeGlobalClientMode( Client::Mode_FullscreenDemo );
	}
	void mapWindowDemo()
	{
		changeGlobalClientMode( Client::Mode_WindowDemo );
	}
	void mapScreenLock()
	{
		changeGlobalClientMode( Client::Mode_Locked );
	}


private:
	virtual void closeEvent( QCloseEvent * _ce );

	QGraphicsScene * m_workspace;

	QButtonGroup * m_modeGroup;

	QList<QAction *> m_sysTrayActions;

	QWidget * m_sideBarWidget;
	int m_openedTabInSideBar;

	QReadWriteLock m_rctrlLock;
	RemoteControlWidget * m_remoteControlWidget;
	bool m_stopDemo;
	int m_remoteControlScreen;

	OverviewWidget * m_overviewWidget;
	ClassroomManager * m_classroomManager;
	UserList * m_userList;
	SnapshotList * m_snapshotList;
	ConfigWidget * m_configWidget;

	friend class ClassroomManager;

} ;


extern QSplashScreen * splashScreen;


#endif
