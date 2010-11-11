/*
 * ClassroomManager.cpp - implementation of classroom manager
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


#include <italcconfig.h>
#include <math.h>

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtNetwork/QHostInfo>


#include "ClassroomManager.h"
#include "Client.h"
#include "ClientAction.h"
#include "Dialogs.h"
#include "ItalcConfiguration.h"
#include "LocalSystem.h"
#include "MasterCore.h"
#include "MainWindow.h"
#include "PersonalConfig.h"



template<typename T>
inline T roundCorrect( T _val )
{
	if( _val - floor( _val ) < 0.5 )
	{
		return( floor( _val ) );
	}
	return( ceil( _val ) );
}






ClassroomManager::ClassroomManager() :
	QObject(),
	m_globalClientMode( Client::Mode_Overview )
{
}




ClassroomManager::~ClassroomManager()
{
}



/*
// routine that returns m_view of all active clients
ClientList ClassroomManager::visibleClients() const
{
	QVector<Client *> vc;
	for( int i = 0; i < m_view->topLevelItemCount(); ++i )
	{
		getVisibleClients( m_view->topLevelItem( i ), vc );
	}

	return( vc );
}




void ClassroomManager::getVisibleClients( QTreeWidgetItem * _p,
						QVector<client *> & _vc )
{
	classRoomItem * l = NULL;

	for( int i = 0; i < _p->childCount(); ++i )
	{
		QTreeWidgetItem * lvi = _p->child( i );
		if( lvi->childCount() )
		{
			getVisibleClients( lvi, _vc );
		}
		else if( ( l = dynamic_cast<classRoomItem *>( lvi ) ) != NULL &&
								l->isVisible() )
		{
			_vc.push_back( l->getClient() );
		}
	}
}
*/



void ClassroomManager::updateClients()
{
	ClientList clients = activeClients();

	foreach( Client * cl, clients )
	{
		// update current client
		cl->update();
	}

	QTimer::singleShot( MasterCore::personalConfig->clientUpdateInterval() *
				1000, this, SLOT( updateClients() ) );
}




void ClassroomManager::changeGlobalClientMode( int _mode )
{
	Client::Modes newMode = static_cast<Client::Modes>( _mode );
	if( newMode != m_globalClientMode || newMode == Client::Mode_Overview )
	{
		m_globalClientMode = newMode;
		ClientList clients = activeClients();

		foreach( Client * cl, clients )
		{
			cl->changeMode( m_globalClientMode );
		}
	}
}




void ClassroomManager::powerOnClients()
{
	ClientAction action( ItalcCore::PowerOnComputer, this );
	action.process( activeClients(), ClientAction::VisibleClients );
}




void ClassroomManager::remoteLogon()
{
	ClientAction action( ItalcCore::LogonUserCmd, this );
	action.process( activeClients(), ClientAction::VisibleClients );
}




void ClassroomManager::powerDownClients()
{
	ClientAction action( ItalcCore::PowerDownComputer, this );
	action.process( activeClients(), ClientAction::VisibleClients );
}




void ClassroomManager::directSupport()
{
	const QString h = SupportDialog::getHost( MasterCore::mainWindow );
	if( !h.isEmpty() )
	{
		MasterCore::mainWindow->remoteControlDisplay( h );
	}
}




void ClassroomManager::sendMessage()
{
	ClientAction action( ItalcCore::DisplayTextMessage, this );
	action.process( activeClients(), ClientAction::VisibleClients );
}



/*
void ClassroomManager::clientMenuRequest()
{
	bool fullMenu = ( selectedItems().size() == 1 );

	QMenu * menu = new clientMenu( tr( "Actions" ), m_clientMenu->actions(),
					this, fullMenu );
	connect( menu, SIGNAL( triggered( QAction * ) ),
		this, SLOT( clientMenuTriggered( QAction * ) ) );

	menu->exec( QCursor::pos() );

	delete menu;
}




void ClassroomManager::clientMenuTriggered( QAction * _action )
{
	QVector<Client *> clients;

	foreach ( classRoomItem * cri, selectedItems() )
	{
		clients.append( cri->getClient() );
	}

	ClientAction::process( _action, clients );
}




QVector<classRoomItem *> ClassroomManager::selectedItems()
{
	QVector<classRoomItem *> vc;

	for( int i = 0; i < m_view->topLevelItemCount(); ++i )
	{
		getSelectedItems( m_view->topLevelItem( i ), vc );
	}

	// Move the currentItem to the beginning of the list
	classRoomItem * current = dynamic_cast<classRoomItem *>( m_view->currentItem() );
	if ( vc.contains( current ) )
	{
		vc.remove( vc.indexOf( current ));
		vc.push_front( current );
	}

	return( vc );
}
*/


/*
void ClassroomManager::getSelectedItems( QTreeWidgetItem * _p,
					QVector<classRoomItem *> & _vc,
					bool _add_all )
{
	bool select = _add_all || _p->isSelected();
	if( _p->childCount() )
	{
		for( int i = 0; i < _p->childCount(); ++i )
		{
			getSelectedItems( _p->child( i ), _vc, select );
		}
	}
	else if( dynamic_cast<classRoomItem *>( _p ) && select )
	{
		_vc.push_back( dynamic_cast<classRoomItem *>( _p ) );
	}
}
*/


/*
// slots for client-actions in context-menu
void ClassroomManager::showHideClient()
{
	QVector<classRoomItem *> si = selectedItems();

	if( si.size() > 0 )
	{
		bool all_shown = TRUE;
		foreach( classRoomItem * cri, si )
		{
			if( cri->getClient()->isVisible() )
			{
				all_shown = FALSE;
				break;
			}
		}
		foreach( classRoomItem * cri, si )
		{
			cri->getClient()->setVisible( all_shown );
		}
	}
}




void ClassroomManager::editClientSettings()
{
	QVector<classRoomItem *> si = selectedItems();

	if( si.size() > 0 )
	{
		foreach( classRoomItem * cri, si )
		{
			ClientSettingsDialog settingsDlg( cri->getClient(),
					mainWindow(),
						cri->parent()->text( 0 ) );
			settingsDlg.exec();
		}
		saveGlobalClientConfig();
		savePersonalConfig();
	}
}




void ClassroomManager::removeClient()
{
	QVector<classRoomItem *> si = selectedItems();

	if( si.size() > 0 )
	{
		foreach( classRoomItem * cri, si )
		{
			cri->getClient()->hide();
			m_view->setItemHidden( cri, TRUE );
			m_clientsToRemove.push_back( cri->getClient() );
		}
	}
}




void ClassroomManager::setStateOfClassRoom( classRoom * _cr, bool _shown )
{
	if( _shown )
	{
		_cr->setMenuItemIcon( QIcon( ":/resources/greenled.png" ) );
	}
	else
	{
		_cr->setMenuItemIcon( QIcon() );
	}

	// If all clients are shown, we hide them all. Otherwise we show all.
	for( int i = 0; i < _cr->childCount(); ++i )
	{
		QTreeWidgetItem * cri = _cr->child( i );
		if( dynamic_cast<classRoomItem *>( cri ) != NULL )
		{
			dynamic_cast<classRoomItem *>( cri )->getClient()->
							setVisible( _shown );
		}
	}
}




QAction * ClassroomManager::addClassRoomToQuickSwitchMenu( classRoom * _cr )
{
	QAction * a = new QAction( _cr->text( 0 ), m_quickSwitchMenu );
	connect( a, SIGNAL( triggered( bool ) ), _cr,
						SLOT( switchToClassRoom() ) );
	m_quickSwitchMenu->insertAction( m_qsmClassRoomSeparator, a );
	return( a );
}




void ClassroomManager::showSelectedClassRooms()
{
	foreach( classRoom * cr, m_classRooms )
	{
		if( m_view->isItemSelected( cr ) && cr->childCount() )
		{
			setStateOfClassRoom( cr, TRUE );
		}
	}
}




void ClassroomManager::hideSelectedClassRooms()
{
	foreach( classRoom * cr, m_classRooms )
	{
		if( m_view->isItemSelected( cr ) && ( cr )->childCount() )
		{
			setStateOfClassRoom( cr, FALSE );
		}
	}
}




void ClassroomManager::hideAllClassRooms()
{
	foreach( classRoom * cr, m_classRooms )
	{
		setStateOfClassRoom( cr, FALSE );
	}
}




void ClassroomManager::editClassRoomName()
{
	foreach( classRoom * cr, m_classRooms )
	{
		if( m_view->isItemSelected( cr ) == FALSE )
		{
			continue;
		}
		QString classroom_name = cr->text( 0 );

		bool ok;
		classroom_name = QInputDialog::getText( this,
			tr( "New name for classroom" ),
			tr( "Please enter a new name for classroom \"%1\"." ).
				arg( classroom_name ), QLineEdit::Normal,
							classroom_name, &ok );
		if( ok && !classroom_name.isEmpty() )
		{
			cr->setText( 0, classroom_name );
		}
		saveGlobalClientConfig();
		savePersonalConfig();
	}
}




void ClassroomManager::removeClassRoom()
{
	foreach( classRoom * cr, m_classRooms )
	{
		if( m_view->isItemSelected( cr ) == FALSE )
		{
			continue;
		}
		if( QMessageBox::question( window(), tr( "Remove classroom" ),
			tr( "Are you sure want to remove classroom \"%1\"?\n"
				"All computers in it will be removed as well!" ).
							arg( cr->text( 0 ) ),
							QMessageBox::Yes,
							QMessageBox::No ) ==
					QMessageBox::No )
		{
			continue;
		}

		removeClassRoom( cr );
	}
}




void ClassroomManager::removeClassRoom( classRoom * cr )
{
		m_view->setItemHidden( cr, TRUE );

		for ( int i = 0 ; i < cr->childCount(); ++i )
		{
			if ( classRoomItem * cri =
					dynamic_cast<classRoomItem *>( cr->child( i ) ) )
			{
				Client * cl = cri->getClient();
				cl->hide();
				m_clientsToRemove.push_back( cl );
			}
			else if ( classRoom * childCr =
					dynamic_cast<classRoom *>( cr->child( i ) ) )
			{
				removeClassRoom( childCr );
			}
		}

		m_classRoomsToRemove.push_back( cr );
}




// slots for general actions in context-menu
void ClassroomManager::addClient()
{
	if( m_classRooms.size() == 0 )
	{
		if( QMessageBox::question( window(), tr( "Missing classroom" ),
						tr( "Before adding computers "
							"you have to "
							"create at least one "
							"classroom.\nDo you "
							"want to create a new "
							"classrom now?" ),
						QMessageBox::Yes,
						QMessageBox::No ) ==
							QMessageBox::No )
		{
			return;
		}
		addClassRoom();
		if( m_classRooms.size() == 0 )
		{
			return;
		}
	}

	QString classroom_name = "";

	foreach( classRoom * cr, m_classRooms )
	{
		if( m_view->isItemSelected( cr ) )
		{
			classroom_name = ( cr )->text( 0 );
			break;
		}
	}

	ClientSettingsDialog settingsDlg( NULL, mainWindow(), classroom_name );
	settingsDlg.setWindowTitle( tr( "Add computer" ) );
	settingsDlg.exec();
	saveGlobalClientConfig();
	savePersonalConfig();
}




void ClassroomManager::addClassRoom()
{
	bool ok;
	QString classroom_name = QInputDialog::getText( this,
			tr( "New classroom" ),
			tr( "Please enter the name of the classroom you "
							"want to create." ),
			QLineEdit::Normal, tr( "New classroom" ), &ok );
	if( ok && !classroom_name.isEmpty() )
	{
		classRoom * sel_cr = NULL;
		foreach( QTreeWidgetItem * item, m_view->selectedItems() )
		{
			sel_cr = dynamic_cast<classRoom *>( item );

			if ( !sel_cr && dynamic_cast<classRoomItem *>( item ) )
			{
				sel_cr = dynamic_cast<classRoom *>( 
					item->parent() );
			}

			if ( sel_cr )
			{
				break;
			}
		}

		if( sel_cr != NULL )
		{
			m_classRooms.push_back( new classRoom( classroom_name,
							this, sel_cr ) );
		}
		else
		{
			m_classRooms.push_back( new classRoom( classroom_name,
							this, m_view ) );
		}
		saveGlobalClientConfig();
	}
}




void ClassroomManager::hideTeacherClients()
{
	ClientList clients = activeClients();

	foreach( Client * cl, clients )
	{
		if( cl->type() == Client::Type_Teacher )
		{
			cl->hide();
		}
	}
}




void ClassroomManager::showUserColumn( int _show )
{
	m_view->showColumn( _show ? 2 : 1 );
	m_view->hideColumn( _show ? 1 : 2 );
}




void ClassroomManager::clientVisibleChanged()
{
	if( m_autoArranged == true )
	{
		arrangeWindows();
	}
}
*/






#if 0
classTreeWidget::classTreeWidget( QWidget * _parent ) :
	QTreeWidget( _parent ),
	m_clientPressed( NULL )
{
	setDragEnabled( true );
	setAcceptDrops( true );
	setDropIndicatorShown( true ); 
	setDragDropMode( QAbstractItemView::InternalMove );

	connect( this, SIGNAL( itemSelectionChanged() ),
		this, SLOT( itemSelectionChanged() ) );
}




void classTreeWidget::mousePressEvent( QMouseEvent * _me )
{
	classRoomItem * item = dynamic_cast<classRoomItem *>( itemAt( _me->pos() ) );

	if( item && _me->button() == Qt::LeftButton )
	{
		m_clientPressed = item->getClient();
		m_clientPressed->zoom();
	}

	if( item && _me->button() == Qt::RightButton )
	{
		if ( ! item->isSelected() )
		{
			clearSelection();
			item->setSelected( TRUE );
		}
	}

	QTreeWidget::mousePressEvent( _me );
}




void classTreeWidget::mouseMoveEvent( QMouseEvent * _me )
{
	if ( m_clientPressed ) 
	{
		m_clientPressed->zoomBack();
		m_clientPressed = NULL;
	}

	QTreeWidget::mouseMoveEvent( _me );
}




void classTreeWidget::mouseReleaseEvent( QMouseEvent * _me )
{
	if ( m_clientPressed ) 
	{
		m_clientPressed->zoomBack();
		m_clientPressed = NULL;
	}

	QTreeWidget::mouseReleaseEvent( _me );
}




bool classTreeWidget::droppingOnItself( QTreeWidgetItem * _target )
{
    QList<QTreeWidgetItem *> selected = selectedItems();
    while ( _target )
    {
	    if ( selected.contains( _target ) )
		    return true;
	    _target = dynamic_cast<QTreeWidgetItem * >( _target->parent() );
    }
    return false;
}




void classTreeWidget::dragMoveEvent( QDragMoveEvent * _e )
{
	if ( _e->source() == this ) 
	{
		int clients_selected = 0;
		foreach( QTreeWidgetItem * item, selectedItems() )
		{
			if ( dynamic_cast<classRoomItem *>( item ) )
			{
				clients_selected++;
			}
		}

		QTreeWidgetItem * target = itemAt( _e->pos() );
		
		/* Don't drop clients to the root nor
		 * classroom to its own child */
		if ( ( clients_selected && ! target ) ||
			droppingOnItself( target ) )
		{
			_e->ignore();
		}
		else
		{
			_e->setDropAction( Qt::MoveAction );
			_e->accept();
		}
	}
}




void classTreeWidget::dropEvent( QDropEvent * _e )
{
    if ( _e->source() == this &&
    	dragDropMode() == QAbstractItemView::InternalMove )
    {
		QTreeWidgetItem * target = itemAt( _e->pos() );

		/* Use client's parent as target */
		if ( dynamic_cast<classRoomItem *>( target ))
		{
			target = target->parent();
		}

		/* Workaround for Qt bug #155700 (fixed in Qt 4.3.4) */
		bool sortingEnabled = isSortingEnabled();
		setSortingEnabled( false );

		/* Move selected items */
		foreach ( QTreeWidgetItem * item, selectedItems() )
		{
			if ( item != target )
			{
				QTreeWidgetItem * parent = item->parent();
				if ( parent )
				{
					parent->takeChild( parent->indexOfChild( item ) );
				}
				else
				{
					takeTopLevelItem( indexOfTopLevelItem( item ) );
				}

				if ( target ) 
				{
					target->addChild( item );
				}
				else
				{
					addTopLevelItem( item );
				}
			}
		}

		setSortingEnabled( sortingEnabled );

		_e->accept();
    }
}




/* Update client windows quickly after selections have changed */
void classTreeWidget::itemSelectionChanged()
{

	/* update old selections */
	foreach( QTreeWidgetItem * item, m_selectedItems )
	{
		classRoomItem * cri = dynamic_cast<classRoomItem *>( item );
		if ( cri )
		{
			cri->getClient()->update();
		}
	}

	m_selectedItems = selectedItems();

	/* update new selections - some clients may belong to both
	 * lists and are updated here twice. */
	foreach( QTreeWidgetItem * item, m_selectedItems )
	{
		classRoomItem * cri = dynamic_cast<classRoomItem *>( item );
		if ( cri )
		{
			cri->getClient()->update();
		}
	}
}




/* Set to current but don't select it automatically */
void classTreeWidget::setCurrentItem( QTreeWidgetItem * _item )
{
	QModelIndex index = indexFromItem( _item );

	if ( index.isValid() )
	{
		selectionModel()->setCurrentIndex( index,
				QItemSelectionModel::NoUpdate );
	}
}








classRoom::classRoom( const QString & _name,
					ClassroomManager * _classroom_manager,
						QTreeWidgetItem * _parent ) :
	QTreeWidgetItem( _parent, QStringList( _name ) ),
	m_classroomManager( _classroom_manager ),
	m_qsMenuAction( m_classroomManager->addClassRoomToQuickSwitchMenu(
									this ) )
{
}




classRoom::classRoom( const QString & _name,
					ClassroomManager * _classroom_manager,
						QTreeWidget * _parent ) :
	QTreeWidgetItem( _parent, QStringList( _name ) ),
	m_classroomManager( _classroom_manager ),
	m_qsMenuAction( m_classroomManager->addClassRoomToQuickSwitchMenu(
									this ) )
{
}




classRoom::~classRoom()
{
	delete m_qsMenuAction;
}




void classRoom::clientMenuTriggered( QAction * _action )
{
	QVector<Client *> clients;
	ClassroomManager::getVisibleClients( this, clients );

	ClientAction::process( _action, clients );
}




void classRoom::switchToClassRoom()
{
	m_classroomManager->hideAllClassRooms();
	m_classroomManager->setStateOfClassRoom( this, TRUE );
}










classRoomItem::classRoomItem( Client * _client, QTreeWidgetItem * _parent ) :
	QTreeWidgetItem( _parent, QStringList( _client->name() ) ),
	m_visible( FALSE ),
	m_client( _client )
{
	if( s_clientPixmap == NULL )
	{
		s_clientPixmap = new QPixmap( ":/resources/client_hidden.png" );
	}

	if( s_clientObservedPixmap == NULL )
	{
		s_clientObservedPixmap = new QPixmap(
					":/resources/client_visible.png" );
	}

	setFlags( Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
		Qt::ItemIsEnabled );

	setVisible( m_client->isVisible() );
	setText( 1, m_client->hostname() );
	setUser( m_client->user() );
}




classRoomItem::~classRoomItem()
{
	m_client->m_classRoomItem = NULL;
}




void classRoomItem::setVisible( const bool _obs )
{
	m_visible = _obs;
	if( _obs == FALSE )
	{
		setIcon( 0, *s_clientPixmap );
	}
	else
	{
		setIcon( 0, *s_clientObservedPixmap );
	}
}




void classRoomItem::setUser( const QString & _name )
{
	setText( 2, _name );
}


#endif

