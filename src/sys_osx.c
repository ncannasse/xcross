/* ************************************************************************ */
/*																			*/
/*  XCross - Neko Standalone Runtime										*/
/*  Copyright (c)2006 Motion-Twin											*/
/*																			*/
/* This library is free software; you can redistribute it and/or			*/
/* modify it under the terms of the GNU Lesser General Public				*/
/* License as published by the Free Software Foundation; either				*/
/* version 2.1 of the License, or (at your option) any later version.		*/
/*																			*/
/* This library is distributed in the hope that it will be useful,			*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of			*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU		*/
/* Lesser General Public License or the LICENSE file for more details.		*/
/*																			*/
/* ************************************************************************ */
#include <Carbon/Carbon.h>
#include "sys.h"

void sys_init() {
}

int sys_dialog( const char *title, const char *message, int flags ) {
	CFOptionFlags result;
	CFUserNotificationDisplayAlert(
		0, (flags & DLG_ERROR) ? 0 : kCFUserNotificationCautionAlertLevel,
		0, 0, 0,
		CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8 ),
		CFStringCreateWithCString(NULL, message, kCFStringEncodingUTF8 ),
		(flags & DLG_CONFIRM ) ? CFSTR("Yes") : NULL,
		(flags & DLG_CONFIRM ) ? CFSTR("No") : NULL,
		NULL,
		&result
	);
	return (flags & DLG_CONFIRM) && (result == kCFUserNotificationDefaultResponse);
}

/* ************************************************************************ */
