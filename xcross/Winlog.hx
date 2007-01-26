package xcross;

class Winlog {

	var w : Void;
	public var text(default,setText) : String;
	public var button(default,setButton) : String;
	public var enabled(default,setEnabled) : Bool;

	public function new( title : String ) {
		var me = this;
		w = winlog_new(untyped title.__s,function() { me.onClick(); });
		text = "";
		button = "";
		enabled = false;
	}

	public function onClick() {
	}

	public function log( line : String ) {
		text += line + "\n";
	}

	public function destroy() {
		winlog_destroy(w);
	}

	function setText(t) {
		text = t;
		winlog_set(w,untyped t.__s);
		return t;
	}

	function setButton(t) {
		button = t;
		winlog_set_button(w,untyped t.__s,if( enabled == null ) false else enabled);
		return t;
	}

	function setEnabled(e) {
		enabled = e;
		winlog_set_button(w,untyped button.__s,e);
		return e;
	}

	static var winlog_new = neko.Lib.load("xcross","os_winlog_new",2);
	static var winlog_set = neko.Lib.load("xcross","os_winlog_set",2);
	static var winlog_set_button = neko.Lib.load("xcross","os_winlog_set_button",3);
	static var winlog_destroy = neko.Lib.load("xcross","os_winlog_destroy",1);

}