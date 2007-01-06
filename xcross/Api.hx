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
package xcross;

class Api {

	public static function message( title : String, message : String ) {
		untyped os_dialog(title.__s,message.__s,false,false);
	}

	public static function error( title : String, message : String ) {
		untyped os_dialog(title.__s,message.__s,true,false);
	}

	public static function confirm( title : String, message : String ) : Bool {
		return untyped os_dialog(title.__s,message.__s,false,true);
	}

	static var os_dialog = neko.Lib.load("xcross","os_dialog",4);

}

/* ************************************************************************ */
