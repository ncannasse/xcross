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

DEFINE_PRIM(os_dialog,4);

/* ************************************************************************ */
