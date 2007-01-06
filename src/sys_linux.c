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
#include <gtk/gtk.h>
#include "sys.h"

void sys_init() {
	gtk_init(NULL,NULL);
}

int sys_dialog( const char *title, const char *message, int flags ) {
	gint ret;
	GtkWidget *dialog = gtk_message_dialog_new(
		NULL,
		0, 
		(flags & DLG_ERROR) ? GTK_MESSAGE_ERROR : GTK_MESSAGE_INFO, 
		(flags & DLG_CONFIRM) ? GTK_BUTTONS_YES_NO : GTK_BUTTONS_OK,
		"%s", title
	);
	gtk_message_dialog_format_secondary_text(GTK_DIALOG(dialog),"%s",message);
	ret = gtk_dialog_run( GTK_DIALOG(dialog) );
	gtk_widget_destroy( dialog );
	return (ret == GTK_RESPONSE_YES);
}

/* ************************************************************************ */
