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
#include <pthread.h>
#include "sys.h"

pthread_t main_thread_id;

void sys_init() {
	XInitThreads();
	gtk_init(NULL,NULL);
	main_thread_id = pthread_self();
}

void sys_loop() {
	gtk_main();
}

void sys_stop() {
	gtk_main_quit();
}

typedef struct {
	sys_callback callback;
	void *param;
} sig_data;

static gint gtk_event( GtkWidget *_, sig_data *_d ) {
	sig_data *d = (sig_data*)_d;
	d->callback( d->param );
	return 0;
}

void sys_sync( sys_callback cb, void *param ) {
	sc_data *d = (sc_data*)malloc(sizeof(sc_data));
	d->callback = cb;
	d->param = param;
	gtk_idle_add( cb, (gpointer)param );
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

typedef struct {
	GtkWidget *wnd;
	GtkWidegt *panel;
	sig_data click;
} winlog;


static gboolean cancel_delete( GtkWidget *widget, GdkEvent  *event, gpointer data ) {
	return TRUE;
}

void *sys_winlog_new( const char *title, sys_callback cb, void *param ) {
	winlog *w = (winlog*)malloc(sizeof(winlog));
	int width = 400, height = 300;

	// window
	w->wnd = gtk_window_new(GTW_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(w->wnd),title);
	gtk_container_set_border_width(GTK_CONTAINER(w->wnd),5);
	gtk_window_set_default_size(GTK_WINDOW(w->wnd),width,height);
	g_signal_connect(G_OBJECT(w->wnd),"delete_event",G_CALLBACK(cancel_delete),NULL);

	// panel
	w->panel = gtk_vbox_new (FALSE,2);	
	gtk_container_add(GTK_CONTAINER(window),w->panel);	

	// text
	w->text = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(w->panel),w->text,1,1,0);
	gtk_text_view_set_editable(GTK_TEXTVIEW(w->text),FALSE);

	// button
	w->button = gtk_button_new();
	w->click.callback = cb;
	w->click.param = param;
    g_signal_connect(G_OBJECT(w->button),"clicked",G_CALLBACK(move_button),(gpointer)&w->click);
	gtk_box_pack_start(GTK_BOX(w->panel),w->button,0,0,0);

	// show
    gtk_widget_show_all(w->wnd);
	return w;
}

void sys_winlog_set( void *_wnd, const char *txt ) {
	window *w = (window*)_wnd;
	GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->text));
	gtk_text_buffer_set_text(buf,txt);
}

void sys_winlog_set_button( void *_wnd, const char *txt, int enabled ) {
	window *w = (window*)_wnd;
	gtk_button_set_label(GTK_BUTTON(w->button),txt);
	gtk_widget_set_sensitive(w->button,enabled);
}

void sys_winlog_destroy( void *_wnd ) {
	window *w = (window*)_wnd;
	gtk_widget_destroy(w->window);
	free(w);
}

/* ************************************************************************ */
