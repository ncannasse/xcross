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
		syncLock(callback(os_dialog,untyped title.__s,untyped message.__s,false,false));
	}

	public static function error( title : String, message : String ) {
		syncLock(callback(os_dialog,untyped title.__s,untyped message.__s,true,false));
	}

	public static function confirm( title : String, message : String ) : Bool {
		return syncLock(callback(os_dialog,untyped title.__s,untyped message.__s,false,true));
	}

	public static function loop() {
		os_loop();
	}

	public static function stop() {
		os_stop();
	}

	public static function sync( f : Void -> Void ) {
		os_sync(f);
	}

	public static function syncLock<T>( f : Void -> T ) : T {
		if( os_is_main_thread() )
			return f();
		var l = new neko.vm.Lock();
		var r;
		var exc = null;
		os_sync(function() {
			try {
				r = f();
			} catch( e : Dynamic ) {
				exc = { v : e };
			}
			l.release();
		});
		l.wait();
		if( exc != null )
			throw exc.v;
		return r;
	}

	static var os_is_main_thread = neko.Lib.load("xcross","os_is_main_thread",0);
	static var os_dialog : Void -> Void -> Bool -> Bool -> Bool = neko.Lib.load("xcross","os_dialog",4);
	static var os_loop = neko.Lib.load("xcross","os_loop",0);
	static var os_stop = neko.Lib.load("xcross","os_stop",0);
	static var os_sync = neko.Lib.load("xcross","os_sync",1);

}

/* ************************************************************************ */
