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
#include "sys.h"

void sys_init() {
}

int sys_dialog( const char *title, const char *message, int flags ) {
	return MessageBox(NULL,message,title,((flags & DLG_CONFIRM)?MB_YESNO:MB_OK) | ((flags & DLG_ERROR)?MB_ICONERROR:MB_ICONINFORMATION) ) == IDYES;
}

extern int main( int argc, char *argv[] );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	char *argv[] = { "", lpCmdLine };
	return main(*lpCmdLine?2:1,argv);
}

/* ************************************************************************ */
