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
#include "sys.h"
#include <neko.h>

typedef struct {
	void *p;
	value click;
} window;

#define val_window(v)		((window*)val_data(v))

DEFINE_KIND(k_winlog);

static value os_dialog( value title, value msg, value error, value confirm ) {
	int r;
	val_check(title,string);
	val_check(msg,string);
	val_check(error,bool);
	val_check(confirm,bool);
	r = sys_dialog(
		val_string(title),
		val_string(msg),
		(val_bool(error)?DLG_ERROR:0) | (val_bool(confirm)?DLG_CONFIRM:0)
	);
	return alloc_bool(r);
}

static value os_is_main_thread() {
	return alloc_bool(sys_is_main_thread());
}

static value os_loop() {
	if( !sys_is_main_thread() )
		neko_error();
	sys_loop();
	return val_null;
}

static value os_stop() {
	sys_stop();
	return val_null;
}

static void *sync_callback( void *_r ) {
	value *r = (value*)_r;
	value f = *r;
	free_root(r);
	val_call0(f);
	return NULL;	
}

static void *wnd_callback( void *_w ) {
	window *w = (window*)_w;
	val_call0(w->click);
	return NULL;
}

static value os_sync( value f ) {
	value *r;
	val_check_function(f,0);
	r = alloc_root(1);
	*r = f;
	sys_sync(sync_callback,r);
	return val_null;
}

static value os_winlog_new( value title, value f ) {
	window *w;	
	val_check(title,string);
	val_check_function(f,0);
	w = (window*)alloc(sizeof(window));
	w->click = f;
	w->p = sys_winlog_new(val_string(title),wnd_callback,w);
	if( w->p == NULL )		
		neko_error();	
	return alloc_abstract(k_winlog,w);
}

static value os_winlog_set( value wnd, value txt ) {
	val_check_kind(wnd,k_winlog);
	val_check(txt,string);
	sys_winlog_set(val_window(wnd)->p,val_string(txt));
	return val_null;
}

static value os_winlog_set_button( value wnd, value txt, value enabled ) {
	val_check_kind(wnd,k_winlog);
	val_check(txt,string);
	val_check(enabled,bool);
	sys_winlog_set_button(val_window(wnd)->p,val_string(txt),val_bool(enabled));
	return val_null;
}

static value os_winlog_destroy( value wnd ) {
	val_check_kind(wnd,k_winlog);
	sys_winlog_destroy(val_window(wnd)->p);
	val_kind(wnd) = NULL;
	return val_null;
}

DEFINE_PRIM(os_is_main_thread,0);
DEFINE_PRIM(os_dialog,4);
DEFINE_PRIM(os_loop,0);
DEFINE_PRIM(os_sync,1);
DEFINE_PRIM(os_stop,0);
DEFINE_PRIM(os_winlog_new,2);
DEFINE_PRIM(os_winlog_set,2);
DEFINE_PRIM(os_winlog_set_button,3);
DEFINE_PRIM(os_winlog_destroy,1);

/* ************************************************************************ */
