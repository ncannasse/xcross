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

class Run {

	static function build(file,content,os) {
		var exe_content = neko.io.File.getContent("bin/xcross_"+os);
		var p = new neko.io.Path(file);
		if( os == "win" )
			p.ext = "exe";
		else
			p.ext = null;
		p.file += "-"+os;
		var out = neko.io.File.write(p.toString(),true);
		// similar to nekotools "boot"
		var it = exe_content.split("##BOOT_POS\000\000\000\000##").iterator();
		for( part in it ) {
			out.write(part);
			if( it.hasNext() ) {
				out.write("##BOOT_POS");
				out.writeInt32(exe_content.length);
				out.write("##");
			}
		}
		out.write(content);
		out.close();
	}

	static function main() {
		var args = neko.Sys.args();
		var file = args.shift();
		if( file == null ) {
			neko.Lib.print("Please specify a .n file to package");
			neko.Sys.exit(1);
		}
		var content = try
			neko.io.File.getContent(file)
		catch( e : Dynamic ) try {
			file = args.shift()+file;
			neko.io.File.getContent(file);
		} catch( e : Dynamic ) {
			neko.Lib.print("Could not find file '"+file+"', please specify absolute path");
			neko.Sys.exit(2);
			"";
		}
		build(file,content,"win");
		build(file,content,"osx");
		build(file,content,"linux");
	}
}

/* ************************************************************************ */
