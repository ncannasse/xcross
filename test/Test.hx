class Test {

	static var w : xcross.Winlog;

	static function thread() {
		var i = 0;
		var s = "";
		while( true ) {
			s += Std.string(i++);
			xcross.Api.sync(callback(w.log,s));
			neko.Sys.sleep(0.5);
			w.enabled = !w.enabled;
		}
	}

	static function main() {
		w = new xcross.Winlog("TestLog");
		w.button = "Close";
		w.enabled = true;
		w.onClick = function() {
			if( xcross.Api.confirm("Warning","Do you want to quit ?") )
				xcross.Api.stop();
			else
				xcross.Api.error("Error","Exit Canceled");
		}
		neko.vm.Thread.create(thread);
		xcross.Api.loop();
	}
}