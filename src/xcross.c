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
#include <neko_vm.h>
#include "sys.h"

static field id_primitives;

#define PRIM(module,name,nargs) DECLARE_PRIM(name,nargs)
#include "primitives.h"
#undef PRIM

static value init_primitives() {
	value primitives = alloc_object(NULL);
#	define PRIM(module,name,nargs)	alloc_field(primitives,val_id(#module "@" #name "$" #nargs), alloc_function( name##__##nargs(), nargs, #module "@" #name ))
#	include "primitives.h"
	return primitives;
}

extern void std_main();
extern void regexp_main();
extern void zlib_main();
extern void os_main();

void neko_installer_init() {
	sys_init();
	std_main();
	regexp_main();
	zlib_main();
	os_main();
	id_primitives = val_id("primitives");
}

void neko_installer_error( const char *error ) {
	sys_dialog("Error",error,DLG_ERROR);
}

static value loadprim( value prim, value nargs ) {
	value o = val_this();
	value prims;
	value p;
	buffer b;
	val_check(o,object);
	val_check(prim,string);
	val_check(nargs,int);
	prims = val_field(o,id_primitives);
	val_check(prims,object);
	b = alloc_buffer(NULL);
	val_buffer(b,prim);
	buffer_append_char(b,'$');
	val_buffer(b,nargs);
	p = val_field(prims,val_id(val_string(buffer_to_string(b))));
	if( val_is_null(p) ) {
		buffer b = alloc_buffer("Primitive not found : ");
		val_buffer(b,prim);
		buffer_append(b,"(");
		val_buffer(b,nargs);
		buffer_append(b,")");
		bfailure(b);
	}
	return p;
}

value neko_installer_loader( char *argv[], int argc ) {
	value l = neko_default_loader(argv,argc);
	alloc_field(l,id_primitives,init_primitives());
	alloc_field(l,val_id("loadprim"),alloc_function(loadprim,2,"loadprim"));
	return l;
}

/* ************************************************************************ */
