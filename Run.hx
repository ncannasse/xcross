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
import neko.io.File.FileSeek;

class Run {

	static function build(file,content,os,console) {
		var exe_content = neko.io.File.getBytes("bin/xcross-"+os+(console?"-console":""));
		var p = new neko.io.Path(file);
		if( os == "win" )
			p.ext = "exe";
		else
			p.ext = null;
		p.file += "-"+os;
		var exe_file = p.toString();
		var out = neko.io.File.write(exe_file,true);
		out.write(exe_content);
		out.write(content);
		out.writeString("NEKO");
		out.writeUInt30(exe_content.length);
		out.close();
		return exe_file;
	}

	static function createDir( d ) {
		if( !neko.FileSystem.exists(d) )
			neko.FileSystem.createDirectory(d);
	}

	static var bundle_xml = '<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>English</string>
	<key>CFBundleExecutable</key>
	<string>::exe::</string>
	<key>CFBundleIdentifier</key>
	<string>com.motion-twin.xcross.::name::</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleName</key>
	<string>::name::</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleSignature</key>
	<string>::sign::</string>
	<key>CFBundleVersion</key>
	<string>::version::</string>
	<key>CSResourcesFileMapped</key>
	<true/>
	<key>LSMultipleInstancesProhibited</key>
	<false/>
</dict>
</plist>';

	static function createBundle( name, exe ) {
		var path = new neko.io.Path(exe);
		var exe_name = path.file;
		path.ext = "app";
		path.file = name;
		var dir = path.toString();
		createDir(dir);
		createDir(dir+"/Contents");
		createDir(dir+"/Contents/MacOS");
		var bundle_exe = dir+"/Contents/MacOS/"+exe_name;
		var f = neko.io.File.write(bundle_exe,true);
		var content = neko.io.File.getContent(exe);
		var sign = haxe.Md5.encode(content);
		f.writeString(content);
		f.close();
		neko.Sys.command('chmod +x "'+bundle_exe+'"');
		var inf = new haxe.Template(bundle_xml).execute({
			exe : exe_name,
			name : name,
			sign : sign,
			version : "1.0",
		});
		var f = neko.io.File.write(dir+"/Contents/Info.plist",true);
		f.writeString(inf);
		f.close();
		var f = neko.io.File.write(dir+"/Contents/PkgInfo",true);
		f.writeString("APPL");
		f.writeString(sign);
		f.close();
	}

	static function setConsoleMode( exe, console ) {
		var f = neko.io.File.read(exe,true);
		f.seek(0x3C,SeekBegin);
		var pe_pos = f.readUInt16();
		f.seek(pe_pos,SeekBegin);
		if( f.readString(2) != "PE" )
			throw "Invalid PE header";
		f.seek(0,SeekBegin);
		var begin = f.read(pe_pos + 0x5C);
		f.readByte();
		var end = f.readAll();
		f.close();
		var f = neko.io.File.write(exe,true);
		f.write(begin);
		f.writeByte(if( console ) 0x03 else 0x02);
		f.write(end);
		f.close();
	}

	static function main() {
		var args = neko.Sys.args();
		var forwin = true, forosx = true, forlinux = true;
		var execute = false;
		var console_mode = false;
		var bundle_name = null;
		while( true ) {
			var a = args.shift();
			switch( a ) {
			case "-x":
				execute = true;
			case "-win":
				forosx = false;
				forlinux = false;
			case "-osx":
				forwin = false;
				forlinux = false;
			case "-bundle":
				bundle_name = args.shift();
			case "-console":
				console_mode = true;
			case "-linux":
				forwin = false;
				forosx = false;
			default:
				args.unshift(a);
				break;
			}
		}

		var file = args.shift();
		if( file == null ) {
			neko.Lib.print("Please specify a .n file to package");
			neko.Sys.exit(1);
		}
		var content = try
			neko.io.File.getBytes(file)
		catch( e : Dynamic ) try {
			file = args.pop()+file;
			neko.io.File.getBytes(file);
		} catch( e : Dynamic ) {
			neko.Lib.print("Could not find file '"+file+"', please specify absolute path");
			neko.Sys.exit(2);
			null;
		}
		var exe_win = if( forwin ) build(file,content,"win",false) else null;
		var exe_osx = if( forosx ) build(file,content,"osx",false) else null;
		var exe_linux = if( forlinux ) build(file,content,"linux",console) else null;
		var system = neko.Sys.systemName();
		var win = (system == "Windows");
		if( !win ) {
			if( forwin ) neko.Sys.command("chmod +x "+exe_win);
			if( forosx ) neko.Sys.command("chmod +x "+exe_osx);
			if( forlinux ) neko.Sys.command("chmod +x "+exe_linux);
		}
		if( forwin && console_mode )
			setConsoleMode(exe_win,true);
		if( forosx && bundle_name != null )
			createBundle(bundle_name,exe_osx);
		if( execute ) {
			var cmd = switch( system ) {
			case "Mac": exe_osx;
			case "Windows": exe_win;
			case "Linux": exe_linux;
			default: throw "Unknown system";
			}
			if( cmd == null )
				throw "Can't run since it's not built";
			var p = new neko.io.Path(cmd);
			if( p.dir != null ) {
				neko.Sys.setCwd(p.dir);
				p.dir = if( win ) null else ".";
			}
			if( system == "Mac" && bundle_name != null )
				neko.Sys.command('open "'+bundle_name+'.app"');
			else
				neko.Sys.command( p.toString() );
		}
	}
}

/* ************************************************************************ */
