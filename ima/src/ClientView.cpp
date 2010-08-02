/*
 * ClientView.cpp - implementation of class ClientView which is resposible
 *                  for the view of a client
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

 
#include <QtGui/QLinearGradient>
#include <QtGui/QMenu>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QGraphicsSceneMouseEvent>


#include "Client.h"
#include "ClientView.h"
#include "MasterCore.h"
#include "PersonalConfig.h"



const QSize DEFAULT_CLIENT_SIZE( 256, 192 );
const int DECO_WIDTH = 4;
const int TITLE_HEIGHT = 23;
const QPoint CONTENT_OFFSET( DECO_WIDTH, DECO_WIDTH + TITLE_HEIGHT ); 
const QSize CONTENT_SIZE_SUB( 2*DECO_WIDTH, 2*DECO_WIDTH + TITLE_HEIGHT ); 



ClientView::ClientView( Client * _c ) :
	QGraphicsPixmapItem(),
	m_client( _c ),
	m_clickPoint( -1, -1 ),
	m_origPos( -1, -1 )
{
}




ClientView::~ClientView()
{
	// TODO: set Client model into a defined state
}




void ClientView::contextMenuEvent( QGraphicsSceneContextMenuEvent * )
{
	/* classRoomManager handles ClientMenu */
//	m_mainWindow->getClassroomManager()->clientMenuRequest();
}




void ClientView::zoom()
{
}




void ClientView::zoomBack()
{
}



#if 0
void Client::enlarge()
{
	if ( m_origSize.isValid() ) 
	{
		QWidget * workspace = m_mainWindow->workspace();
		QSize s = QSize( m_origSize );
		
		s.scale( workspace->parentWidget()->size(),
							Qt::KeepAspectRatio );
		setFixedSize( s );

		/* centralize */
		QSize offset = ( workspace->parentWidget()->size() - s ) / 2;
		move( offset.width() - workspace->x(),
			offset.height() - workspace->y() );

		raise();
	}
}




void Client::zoom()
{
	m_origPos = pos();
	m_origSize = size();
	/* Delay zooming before we are sure that
	 * this is not just a fast click.
	 */
	QTimer::singleShot( 300, this, SLOT( enlarge() ) );
}




void Client::zoomBack()
{
	if ( m_origSize.isValid() )
	{	
		move( m_origPos );
		setFixedSize( m_origSize );
		/* reset value: */
		m_origSize = QSize();
	}
}
#endif



void ClientView::mousePressEvent( QGraphicsSceneMouseEvent * _me )
{
/*	classTreeWidget * tree = static_cast<classTreeWidget *>(
				m_classRoomItem->treeWidget() );

	tree->setCurrentItem( m_classRoomItem );*/

	if( _me->button() == Qt::LeftButton )
	{
		static int zvalue = 0;
		setZValue( ++zvalue );
		m_clickPoint = _me->screenPos();
		m_origPos = pos();

		if ( ! ( _me->modifiers() & ( Qt::ControlModifier | Qt::ShiftModifier ) ))
		{
//			tree->clearSelection();
		}
//		m_classRoomItem->setSelected( ! m_classRoomItem->isSelected() );

		zoom();

		_me->ignore();
	}
	else if ( _me->button() == Qt::RightButton )
	{
		if ( ! client()->isSelected() ) {
//			tree->clearSelection();
//			m_classRoomItem->setSelected( true );
		}
	}

	QGraphicsPixmapItem::mousePressEvent( _me );
}




void ClientView::mouseMoveEvent( QGraphicsSceneMouseEvent * _me )
{
	if( m_clickPoint.x() >= 0 )
	{
		zoomBack();
		setPos( m_origPos + _me->screenPos() - m_clickPoint );
		//parentWidget()->updateGeometry();
	}
	else
	{
		QGraphicsPixmapItem::mouseMoveEvent( _me );
	}
}




void ClientView::mouseReleaseEvent( QGraphicsSceneMouseEvent * _me )
{
	zoomBack();
	m_clickPoint = QPoint( -1, -1 );
	QGraphicsPixmapItem::mouseReleaseEvent( _me );
}




void ClientView::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * )
{
	if( MasterCore::personalConfig->clientDoubleClickAction() == 0 )
	{
		client()->remoteControl();
	}
	else
	{
		client()->viewLive();
	}
}



#if 0
void Client::paintEvent( QPaintEvent * _pe )
{
	static QImage * img_unknown = NULL;
	static QImage * img_no_user = NULL;
	static QImage * img_host_unreachable = NULL;
	static QImage * img_demo = NULL;
	static QImage * img_locked = NULL;

	if( img_unknown == NULL )
		img_unknown = new QImage( ":/resources/error.png" );
	if( img_no_user == NULL )
		img_no_user = new QImage( ":/resources/no_user.png" );
	if( img_host_unreachable == NULL )
		img_host_unreachable = new QImage( ":/resources/host_unreachable.png" );
	if( img_demo == NULL )
		img_demo = new QImage( ":/resources/window_demo.png" );
	if( img_locked == NULL )
		img_locked = new QImage( ":/resources/locked.png" );

	QPainter p( this );
	p.setBrush( Qt::white );
	p.setPen( Qt::black );
	p.drawRect( QRect( 0, 0, width()-1, height()-1 ) );
	p.setRenderHint( QPainter::Antialiasing, true );

	p.fillRect( 1, 1, width()-2, TITLE_HEIGHT-2,
			m_classRoomItem->isSelected() ?
				QColor( 96, 96, 96 ) :
						QColor( 224, 224, 224 ) );

	bool showUsername = m_mainWindow->getClassroomManager()->showUsername();
	const QString s = (showUsername && m_user != "") ? m_user :
		( name() + " (" + m_classRoomItem->parent()->text( 0 ) +
									")" );
	QFont f = p.font();
	f.setBold( true );
	p.setFont( f );
	p.setPen( m_classRoomItem->isSelected() ? Qt::white : Qt::black );
	p.drawText( 10, TITLE_HEIGHT-7, s );

	if( m_connection->isConnected() && m_mode == Mode_Overview )
	{
		p.drawImage( CONTENT_OFFSET, m_connection->scaledScreen() );
	}
	else
	{
	const int aw = width() - 2*DECO_WIDTH;
	const int ah = height() - CONTENT_SIZE_SUB.height() - DECO_WIDTH;

	QImage * pm = img_unknown;
	QString msg = tr( "Unknown state" );

	switch( m_state )
	{
		case State_Overview:
			return;
		case State_NoUserLoggedIn:
			pm = img_no_user;
			msg = tr( "No user logged in" );
			break;
		case State_Unreachable:
			pm = img_host_unreachable;
			msg = tr( "Host unreachable" );
			break;
		case State_Demo:
			pm = img_demo;
			msg = tr( "Demo running" );
			break;
		case State_Locked:
			pm = img_locked;
			msg = tr( "Desktop locked" );
			break;
		default:
			break;
	}

	QFont f = p.font();
	f.setBold( true );
	f.setPointSize( f.pointSize() + 1 );
	p.setFont( f );

	QRect r = p.boundingRect( QRect( 5, 0, aw-10, 10 ),
				Qt::TextWordWrap | Qt::AlignCenter, msg );
	QSize s( pm->size() );
	s.scale( aw-10, ah-r.height()-20, Qt::KeepAspectRatio );
	p.drawImage( ( aw-s.width() ) / 2, height()-ah,
						fastQImage( *pm ).scaled( s ) );

	p.setPen( QColor( 0, 0, 0 ) );
	p.drawText( QRect( 5, height()-r.height()-10, aw - 10,
						r.height() ),
				Qt::TextWordWrap | Qt::AlignCenter, msg );
	}

	if( m_makeSnapshot )
	{
		QMutexLocker ml( &m_syncMutex );
		m_makeSnapshot = false;
		if( m_connection->takeSnapshot() )
		{
			s_reloadSnapshotList = true;
		}
	}

}
#endif



