package xcross;
import neko.vm.Ui;

class Winlog {

	var w : Void;
	public var text(default,setText) : String;
	public var button(default,setButton) : String;
	public var enabled(default,setEnabled) : Bool;

	public function new( title : String ) {
		var me = this;
		w = Ui.syncResult(callback(winlog_new,untyped title.__s,function() { me.onClick(); }));
		text = "";
		button = "";
		enabled = false;
	}

	public dynamic function onClick() {
	}

	public function log( line : String ) {
		text += line + "\n";
	}

	public function logProgress( line : String ) {
		var lines = text.split("\n");
		lines.pop();
		lines.push(line);
		text = lines.join("\n");
	}

	public function destroy() {
		Ui.syncResult(callback(winlog_destroy,w));
	}

	function setText(t) {
		text = t;
		Ui.syncResult(callback(winlog_set,w,untyped t.__s));
		return t;
	}

	function setButton(t) {
		button = t;
		Ui.syncResult(callback(winlog_set_button,w,untyped t.__s,if( enabled == null ) false else enabled));
		return t;
	}

	function setEnabled(e) {
		enabled = e;
		Ui.syncResult(callback(winlog_set_button,w,untyped button.__s,e));
		return e;
	}

	static var winlog_new : Void -> (Void -> Void) -> Void = neko.Lib.load("xcross","os_winlog_new",2);
	static var winlog_set : Void -> Void -> Void = neko.Lib.load("xcross","os_winlog_set",2);
	static var winlog_set_button : Void -> Void -> Bool -> Void = neko.Lib.load("xcross","os_winlog_set_button",3);
	static var winlog_destroy : (Void) -> Void = neko.Lib.load("xcross","os_winlog_destroy",1);

}