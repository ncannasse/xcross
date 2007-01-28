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
#include <pthread.h>
#include "res/HIScrollingTextBox.c"
#include "sys.h"

#define xCrossEvent	0xFFFFAA00
#define eCall		0x0

enum {
	pFunc = 'func',
	pParam = 'pram',
};

static pthread_t main_thread_id;

static OSStatus handleEvents( EventHandlerCallRef ref, EventRef e, void *data ) {
	switch( GetEventKind(e) ) {
	case eCall: {
		sys_callback cb;
		void *param;
		GetEventParameter(e,pFunc,typeVoidPtr,0,sizeof(void*),0,&cb);
		GetEventParameter(e,pParam,typeVoidPtr,0,sizeof(void*),0,&param);
		cb(param);
		break; }
	}
	return 0;
}

void sys_init() {
	EventTypeSpec ets[] = {
		{ xCrossEvent, eCall }
	};
	InstallEventHandler(GetApplicationEventTarget(),NewEventHandlerUPP(handleEvents),sizeof(ets) / sizeof(EventTypeSpec),ets,0,0);
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
	SetEventParameter(e,pFunc,typeVoidPtr,sizeof(void*),&cb);
	SetEventParameter(e,pParam,typeVoidPtr,sizeof(void*),&param);
	PostEventToQueue(GetMainEventQueue(),e,kEventPriorityStandard);
	ReleaseEvent(e);
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
	ControlRef button;
	HIViewRef text;
	HIViewRef scroll;
	sys_callback callback;
	void *param;
} winlog;

#define INSTALL_HANDLER(w,c,e,handler,param) {\
		EventTypeSpec ets = {c,e}; \
		InstallWindowEventHandler(w,NewEventHandlerUPP(handler),1,&ets,param,NULL); \
	}

static OSStatus onClose(EventHandlerCallRef next, EventRef e, void *_w ) {
	return noErr;
}

static OSStatus onHit(EventHandlerCallRef next, EventRef e, void *_w ) {
	winlog *w = (winlog*)_w;
	ControlRef c;
	GetEventParameter(e,kEventParamDirectObject,typeControlRef,NULL,sizeof(c),NULL,&c);
	if( c == w->button )
		w->callback(w->param);
	return noErr;
}

void *sys_winlog_new( const char *title, sys_callback cb, void *param ) {
	winlog *w = (winlog*)malloc(sizeof(winlog));
	int width = 400, height = 300;
	Rect wr = { 0, 0, height, width };
	Rect br = { height - 30, width - 90, height - 10, width - 15 };
	CFStringRef str;
	HIViewRef root;

	w->callback = cb;
	w->param = param;

	//  window
	CreateNewWindow(kDocumentWindowClass,
		kWindowStandardHandlerAttribute |
		kWindowStandardDocumentAttributes |
		kWindowCompositingAttribute
	,&wr,&w->wnd);
	str = CFStringCreateWithCString(NULL,title,kCFStringEncodingUTF8);
	SetWindowTitleWithCFString(w->wnd,str);
	CFRelease(str);

	// text
	HIViewFindByID(HIViewGetRoot(w->wnd),kHIViewWindowContentID,&root);
	HIScrollViewCreate(kHIScrollViewOptionsVertScroll,&w->scroll);
	HIViewAddSubview(root,w->scroll);
	HIRect sr;
	sr.origin.x = 0;
	sr.origin.y = 0;
	sr.size.width = width;
	sr.size.height = height - 40;
	HIViewSetFrame(w->scroll,&sr);

	HICreateScrollingTextBox(NULL,CFSTR(""),TRUE,0,0,0,&w->text);
	HIViewAddSubview(w->scroll,w->text);

	// button
	CreatePushButtonControl(w->wnd,&br,CFSTR(""),&w->button);
	
	// events
	INSTALL_HANDLER(w->wnd, kEventClassWindow, kEventWindowClose, onClose, w);
	INSTALL_HANDLER(w->wnd, kEventClassControl, kEventControlHit, onHit, w);

	// display
	RepositionWindow(w->wnd,NULL,kWindowCascadeOnMainScreen);
	HIViewSetVisible(w->text,TRUE);
	HIViewSetVisible(w->scroll,TRUE);
	ShowWindow(w->wnd);
	ActivateWindow(w->wnd,true);
	return w;
}

void sys_winlog_set( void *_w, const char *txt ) {
	winlog *w = (winlog*)_w;
	CFStringRef str = CFStringCreateWithCString(NULL,txt,kCFStringEncodingUTF8);
	SetControlData(w->text,kControlEntireControl,kSetText,sizeof(str),&str);
	CFRelease(str);
	Boolean max = !HIScrollViewCanNavigate(w->scroll,kHIScrollViewScrollToBottom);	
	HIViewRemoveFromSuperview(w->text);
	HIViewAddSubview(w->scroll,w->text);
	if( max )
		HIScrollViewNavigate(w->scroll,kHIScrollViewScrollToBottom);	
}

void sys_winlog_set_button( void *_w, const char *txt, int enabled ) {
	winlog *w = (winlog*)_w;
	CFStringRef str = CFStringCreateWithCString(NULL,txt,kCFStringEncodingUTF8);
	SetControlTitleWithCFString(w->button,str);
	CFRelease(str);
	if( enabled )
		ActivateControl(w->button);
	else
		DeactivateControl(w->button);
}

void sys_winlog_destroy( void *_w ) {
	winlog *w = (winlog*)_w;
	DisposeWindow(w->wnd);
	free(w);
}

/* ************************************************************************ */
