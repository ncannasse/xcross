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

#define DLG_ERROR		1
#define DLG_CONFIRM		2

typedef void *(*sys_callback)( void * );

void sys_init();
void sys_loop();
void sys_stop();
void sys_sync( sys_callback cb, void *param );
int sys_is_main_thread();
int sys_dialog( const char *title, const char *message, int flags );

void *sys_winlog_new( const char *title, sys_callback cb, void *param );
void sys_winlog_set( void *wnd, const char *txt );
void sys_winlog_set_button( void *wnd, const char *txt, int enabled );
void sys_winlog_destroy( void *wnd );

/* ************************************************************************ */
