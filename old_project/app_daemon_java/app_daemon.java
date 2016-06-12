
public class app_daemon {

    static {
	System.loadLibrary("app_daemon");
    }

    public static native void run();
    public static void call() {

	for (;;) {

	    try {

		System.out.println("app_daemon::call");
		Thread.sleep(1000);

	    } catch (InterruptedException e) {

		e.printStackTrace(); 
	    }
	}
    }

    public static void main(String[] args) {

	app_daemon.run();
    }
}



