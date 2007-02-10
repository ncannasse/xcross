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
#include <malloc.h>
#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <locale.h>
#include "sys.h"

static pthread_t main_thread_id;

static gint nothing( gpointer data ) {
	return TRUE;
}

void sys_init() {
	XInitThreads();
	gtk_init(NULL,NULL);
	// keep the loop alive
	gtk_timeout_add( 100, nothing, NULL );
	// prevent atof() from being broken !
	setlocale(LC_NUMERIC,"POSIX");
	main_thread_id = pthread_self();
}

int sys_is_main_thread() {
	return pthread_self() == main_thread_id;
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
	gtk_idle_add( (GtkFunction)cb, (gpointer)param );
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
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),"%s",message);
	ret = gtk_dialog_run( GTK_DIALOG(dialog) );
	gtk_widget_destroy( dialog );
	return (ret == GTK_RESPONSE_YES);
}

typedef struct {
	GtkWidget *wnd;
	GtkWidget *panel;
	GtkWidget *button;
	GtkWidget *text;
	GtkAdjustment *bar;
	sig_data click;
} winlog;


static gboolean cancel_delete( GtkWidget *widget, GdkEvent  *event, gpointer data ) {
	return TRUE;
}

void *sys_winlog_new( const char *title, sys_callback cb, void *param ) {
	winlog *w = (winlog*)malloc(sizeof(winlog));
	int width = 400, height = 300;
	GtkWidget *scroll;

	// window
	w->wnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(w->wnd),title);
	gtk_container_set_border_width(GTK_CONTAINER(w->wnd),5);
	gtk_window_set_default_size(GTK_WINDOW(w->wnd),width,height);
	g_signal_connect(G_OBJECT(w->wnd),"delete_event",G_CALLBACK(cancel_delete),NULL);

	// panel
	w->panel = gtk_vbox_new (FALSE,2);	
	gtk_container_add(GTK_CONTAINER(w->wnd),w->panel);	

	// scroll
	scroll = gtk_scrolled_window_new(NULL,NULL);
	w->bar = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scroll));
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
	gtk_box_pack_start(GTK_BOX(w->panel),scroll,1,1,0);
	
	// text
	w->text = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(scroll),w->text);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(w->text),FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(w->text),GTK_WRAP_WORD_CHAR);

	// button
	w->button = gtk_button_new();
	w->click.callback = cb;
	w->click.param = param;
    g_signal_connect(G_OBJECT(w->button),"clicked",G_CALLBACK(gtk_event),(gpointer)&w->click);
	gtk_box_pack_start(GTK_BOX(w->panel),w->button,0,0,0);

	// show
    gtk_widget_show_all(w->wnd);
	return w;
}

void sys_winlog_set( void *_wnd, const char *txt ) {
	winlog *w = (winlog*)_wnd;
	GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(w->text));
	gdouble pos = w->bar->value;
	int max = (pos + w->bar->page_size) == w->bar->upper;
	gtk_text_buffer_set_text(buf,txt,-1);
	while( gtk_events_pending() )
		gtk_main_iteration();
	if( max )
		pos = w->bar->upper - w->bar->page_size;
	gtk_adjustment_set_value(w->bar,pos);
}

void sys_winlog_set_button( void *_wnd, const char *txt, int enabled ) {
	winlog *w = (winlog*)_wnd;
	gtk_button_set_label(GTK_BUTTON(w->button),txt);
	gtk_widget_set_sensitive(w->button,enabled);
}

void sys_winlog_destroy( void *_wnd ) {
	winlog *w = (winlog*)_wnd;
	gtk_widget_destroy(w->wnd);
	free(w);
}

/* ************************************************************************ */
