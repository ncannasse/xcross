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

#define xCrossEvent	0xFFFFAA00
#define eCall		0x0
#define eFunc		"func"
#define eParam		"param"

static pthread_t main_thread_id;

static OSStatus handleEvents( EventHandlerCallRef ref, EventRef e, void *data ) {
	switch( GetEventKind(e) ) {
	case eCall: {
		sys_callback cb;
		void *param;
		GetEventParameter(evt,eFunc,typeVoidPtr,0,sizeof(void*),0,&cb);
		GetEventParameter(evt,eParam,typeVoidPtr,0,sizeof(void*),0,&param);
		cb(param);
		break;
	}
}

void sys_init() {
	EventTypeSpec ets[] = {
		{ xCrossEvent, eCall }
	};
	InstallEventHandler(GetApplicationEventTarget(),NewEventHandlerUPP(handleEvents),sizeof(elts) / sizeof(EventTypeSpec),ets,0,0);
	main_thread_id =  pthread_self();
}

void sys_loop() {
	RunApplicationEventLoop();
}

void sys_stop() {
	QuitApplicationEventLoop();
}

void sys_sync( sys_callback cb, void *param ) {
	EventRef e;
	CreateEvent(NULL,xCrossEvent,eCall,GetCurrentEventTime(),kEventAttributeUserEvent,&e);
	SetEventParameter(e,eFunc,typeVoidPtr,sizeof(void*),&func);
	SetEventParameter(e,eParam,typeVoidPtr,sizeof(void*),&param);
	PostEventToQueue(GetMainEventQueue(),e,kEventPriorityStandard);
	ReleaseEvent(evt);
}

int sys_dialog( const char *title, const char *message, int flags ) {
	CFOptionFlags result;
	CFUserNotificationDisplayAlert(
		0, (flags & DLG_ERROR) ? kCFUserNotificationStopAlertLevel : kCFUserNotificationNoteAlertLevel,
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

typedef struct {
	WindowRef wnd;
} winlog;

#define INSTALL_HANDLER(w,c,e,handler) {\
		EventTypeSpec ets = {c,e}; \
		InstallWindowEventHandler(w->wnd,NewEventHandlerUPP(handler),1,&ets,w,NULL); \
	}

static OSStatus onClose(EventHandlerCallRef nextHandler, EventRef theEvent, void* u) {
	return noErr;
}

void *sys_winlog_new( const char *title, sys_callback cb, void *param ) {
	winlog *w = (winlog*)malloc(sizeof(winlog));
	int width = 400, height = 300;
	Rect rc = { 0, 0, height, width };
	CreateNewWindow(kDocumentWindowClass,kWindowStandardHandlerAttribute | kWindowStandardDocumentAttributes,&rc,&w->wnd);
	HIWindowChangeFeatures(w->wnd, kWindowCanCollapse | kWindowCanZoom, 0);
	SetWindowTitleWithCFString(CFStringCreateWithCString(0,title,kCFStringEncodingUTF8));
	INSTALL_HANDLER(w, kEventClassWindow, kEventWindowClose, onClose);
	return w;
}

void sys_winlog_set( void *_w, const char *txt ) {
	winlog *w = (winlog*)_w;
}

void sys_winlog_set_button( void *_w, const char *txt, int enabled ) {
	winlog *w = (winlog*)_w;
}

void sys_winlog_destroy( void *_w ) {
	winlog *w = (winlog*)_w;
	DisposeWindow(w->wnd);
	free(w);
}

/* ************************************************************************ */
