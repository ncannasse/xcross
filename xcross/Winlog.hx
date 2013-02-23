package xcross;
import neko.vm.Ui;

class Winlog {

	var w : Dynamic;
	public var text(default,set) : String;
	public var button(default,set) : String;
	public var enabled(default,set) : Bool;

	public function new( title : String ) {
		w = Ui.syncResult(winlog_new.bind(untyped title.__s,function() onClick()));
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
		Ui.syncResult(winlog_destroy.bind(w));
	}

	function set_text(t) {
		text = t;
		Ui.syncResult(winlog_set.bind(w,untyped t.__s));
		return t;
	}

	function set_button(t) {
		button = t;
		Ui.syncResult(winlog_set_button.bind(w,untyped t.__s,if( enabled == null ) false else enabled));
		return t;
	}

	function set_enabled(e) {
		enabled = e;
		Ui.syncResult(winlog_set_button.bind(w,untyped button.__s,e));
		return e;
	}

	static var winlog_new : Dynamic -> (Void -> Void) -> Dynamic = neko.Lib.load("xcross","os_winlog_new",2);
	static var winlog_set : Dynamic -> Dynamic -> Void = neko.Lib.load("xcross","os_winlog_set",2);
	static var winlog_set_button : Dynamic -> Dynamic -> Bool -> Void = neko.Lib.load("xcross","os_winlog_set_button",3);
	static var winlog_destroy : Dynamic -> Void = neko.Lib.load("xcross","os_winlog_destroy",1);

}