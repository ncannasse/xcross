class Test {

	static var w : xcross.Winlog;

	static function thread() {
		var i = 0;
		var s = "";
		try {
			while( true ) {
				s += Std.string(i++);
				w.log(s);
				if( Std.random(10) == 0 )
					w.enabled = !w.enabled;
				neko.Sys.sleep(0.5);
			}
		} catch( e : Dynamic ) {
			xcross.Api.error("Exception",Std.string(e));
		}
	}

	static function main() {
		w = new xcross.Winlog("TestLog");
		w.button = "Close";
		w.enabled = true;
		w.onClick = function() {
			if( xcross.Api.confirm("Warning","Do you want to quit ?") )
				neko.vm.Ui.stopLoop();
			else
				xcross.Api.error("Error","Exit Canceled");
		}
		neko.vm.Thread.create(thread);
		neko.vm.Ui.loop();
	}
}
