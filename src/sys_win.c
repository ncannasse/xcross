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
#include <windows.h>
#include <richedit.h>
#include "sys.h"

#define CLASS_NAME		"XCrossWindow"
#define PTEXT			"_text"
#define PBUTTON			"_button"
#define PCALLBACK		"_callb"
#define PPARAM			"_param"
#define	WM_SYNC_CALL	(WM_USER + 1)

static DWORD main_thread_id = 0;

static LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch( msg ) {
	case WM_CLOSE:
		return 0;
	case WM_COMMAND:
		if( wparam == BN_CLICKED )
			((sys_callback)GetProp(hwnd,PCALLBACK))(GetProp(hwnd,PPARAM));
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}

void sys_init() {
	WNDCLASSEX wcl;
	HINSTANCE hinst = GetModuleHandle(NULL);
	memset(&wcl,0,sizeof(wcl));
	wcl.cbSize			= sizeof(WNDCLASSEX);
	wcl.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcl.lpfnWndProc		= WindowProc;
	wcl.cbClsExtra		= 0;
	wcl.cbWndExtra		= 0;
	wcl.hInstance		= hinst;
	wcl.hIcon			= NULL;
	wcl.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcl.lpszMenuName	= "";
	wcl.lpszClassName	= CLASS_NAME;
	wcl.hIconSm			= 0;
	RegisterClassEx(&wcl);
	main_thread_id = GetCurrentThreadId();
	LoadLibrary("RICHED32.DLL");
}

int sys_is_main_thread() {
	return GetCurrentThreadId() == main_thread_id;
}

int sys_dialog( const char *title, const char *message, int flags ) {
	return MessageBox(NULL,message,title,((flags & DLG_CONFIRM)?MB_YESNO:MB_OK) | ((flags & DLG_ERROR)?MB_ICONERROR:MB_ICONINFORMATION) ) == IDYES;
}

void sys_loop() {
	MSG msg;
	while( GetMessage(&msg,NULL,0,0) ) {
		if( msg.message == WM_SYNC_CALL ) {
			((sys_callback)msg.wParam)((void*)msg.lParam);
			continue;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if( msg.message == WM_QUIT )
			break;
	}
}

void sys_stop() {
	PostThreadMessage(main_thread_id,WM_QUIT,0,0);
}

void sys_sync( sys_callback f, void *param ) {
	PostThreadMessage(main_thread_id,WM_SYNC_CALL,(WPARAM)f,(LPARAM)param);
}

void *sys_winlog_new( const char *title, sys_callback callb, void *param ) {
	HWND wnd, text, but; 
	RECT rc;
	RECT dtop;
	DWORD style = WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION;
	DWORD exstyle = 0;
	HFONT font;
	// SIZE
	int width = 400, height = 300;
	rc.left = 0;
	rc.right = width;
	rc.top = 0;
	rc.bottom = height;	
	AdjustWindowRectEx(&rc, style, FALSE, exstyle);
	GetWindowRect(GetDesktopWindow(),&dtop);
	// WINDOW
	wnd = CreateWindowEx(
		exstyle,
		CLASS_NAME,
		title,
		style,
		(dtop.right - rc.right) / 2,
		(dtop.bottom - rc.bottom) / 2,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	SetProp(wnd,PCALLBACK,callb);
	SetProp(wnd,PPARAM,param);
	// FONT
	{
		LOGFONT f;
		f.lfHeight = -8;
		f.lfWidth = 0;
		f.lfEscapement = 0;
		f.lfOrientation = 0;
		f.lfWeight = FW_NORMAL;
		f.lfItalic = FALSE;
		f.lfUnderline = FALSE;
		f.lfStrikeOut = FALSE;
		f.lfCharSet = DEFAULT_CHARSET;
		f.lfOutPrecision = OUT_DEFAULT_PRECIS;
		f.lfClipPrecision = 0;
		f.lfQuality = DEFAULT_QUALITY;
		f.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		strcpy(f.lfFaceName,"MS Sans Serif");
		font = CreateFontIndirect(&f);
	}
	// TEXT
	text = CreateWindowEx(WS_EX_CLIENTEDGE,"RICHEDIT20A","",ES_MULTILINE | ES_DISABLENOSCROLL | ES_READONLY | WS_VSCROLL | WS_VISIBLE | WS_CHILD,5,5,width - 10,height - 50,wnd,NULL,NULL,NULL);
	SendMessage(text,WM_SETFONT,(WPARAM)font,TRUE);
	SetProp(wnd,PTEXT,text);
	// BUTTON
	but = CreateWindowEx(0,"BUTTON","",WS_VISIBLE | WS_CHILD,width - 80,height - 30,75,25,wnd,NULL,NULL,NULL);
	SendMessage(but,WM_SETFONT,(WPARAM)font,TRUE);
	SetProp(wnd,PBUTTON,but);
	ShowWindow(wnd,SW_SHOW);
	return wnd;
}

void sys_winlog_set( void *wnd, const char *txt ) {
	HWND text = (HWND)GetProp(wnd,PTEXT);
	DWORD a,b;
	SCROLLINFO sinf;
	POINT pt;
	sinf.cbSize = sizeof(sinf);
	sinf.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;	
	GetScrollInfo(text,SB_VERT,&sinf);
	SendMessage(text,EM_GETSCROLLPOS,0,(LPARAM)&pt);
	SendMessage(text,EM_GETSEL,(WPARAM)&a,(LPARAM)&b);
	SetWindowText(text,txt);
	SendMessage(text,EM_SETSEL,a,b);
	if( sinf.nPos + sinf.nPage == sinf.nMax || sinf.nMax == 1 ) {
		GetScrollInfo(text,SB_VERT,&sinf);
		pt.y = sinf.nMax - sinf.nPage;
	}	
	SendMessage(text,EM_SETSCROLLPOS,0,(LPARAM)&pt);
}

void sys_winlog_set_button( void *wnd, const char *txt, int enabled ) {
	HWND but = (HWND)GetProp(wnd,PBUTTON);
	SetWindowText(but,txt);
	EnableWindow(but,enabled);
}

void sys_winlog_destroy( void *wnd ) {
	DestroyWindow(wnd);
}

extern int main( int argc, char *argv[] );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	char *argv[] = { "", lpCmdLine };
	return main(*lpCmdLine?2:1,argv);
}

/* ************************************************************************ */
