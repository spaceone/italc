/*
 * PersonalConfig.h - PersonalConfig class
 *
 * Copyright (c) 2009 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
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

#ifndef _PERSONAL_CONFIG_H
#define _PERSONAL_CONFIG_H

#include "Configuration/Object.h"

class PersonalConfig : public Configuration::Object
{
public:
	PersonalConfig( Configuration::Store::Backend _backend ) :
		Configuration::Object( _backend,
					Configuration::Store::Personal )
	{
	}

	MAP_CONFIG_INT_PROPERTY( demoQuality,
					setDemoQuality,
					"DemoQuality",
					"Behaviour" );
	MAP_CONFIG_INT_PROPERTY( clientUpdateInterval,
					setClientUpdateInterval,
					"ClientUpdateInterval",
					"Behaviour" );
	MAP_CONFIG_INT_PROPERTY( clientDoubleClickAction,
					setClientDoubleClickAction,
					"ClientDoubleClickAction",
					"Behaviour" );
	MAP_CONFIG_INT_PROPERTY( defaultRole,
					setDefaultRole,
					"DefaultRole",
					"Authentication" );
	MAP_CONFIG_INT_PROPERTY( toolButtonIconOnlyMode,
					setToolButtonIconOnlyMode,
					"ToolButtonIconOnlyMode",
					"Interface" );
	MAP_CONFIG_INT_PROPERTY( noToolTips,
					setNoToolTips,
					"NoToolTips",
					"Interface" );
	MAP_CONFIG_PROPERTY( defaultDomain,
					setDefaultDomain,
					"DefaultDomain",
					"Logon" );
} ;

#endif
