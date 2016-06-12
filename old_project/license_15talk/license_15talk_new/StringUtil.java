

public class StringUtil {

    static {

	System.loadLibrary("license_15talk");
    }

    public native byte[] init_start(String path);
    public native void init_callback(byte[] pack);


    public static void printf(String str) {

	System.out.println(str);
    }

    public static void complate() {

	System.out.println("complate");
    }

    public static void demo1() {

	System.out.println("demo1");
    }

    public static void demo2() {

	System.out.println("demo2");
    }

    public static void demo3() {

	System.out.println("demo3");
    }

    public void license_15talk_key_decoding(byte[] buffer) {

	int i, key;
	for (i = 0; i < buffer.length; i++) {

	    if ((i % 3) == 0)
		key = ((i + 1) * 5);
	    else if ((i % 4) == 0)
		key = ((i + 2) / 3);
	    else if ((i % 5) != 0)
		key = ((i + 3) * 4);
	    else
		key = ((i + 4) / 2);

	    if ((key % 2) != 0)
		buffer[i] = (byte) (buffer[i] - key);
	    else
		buffer[i] = (byte) (buffer[i] + key);

	    if ((i % 2) != 0)
		buffer[i] = (byte) ~buffer[i];
	}
    }


    public static void main(String[] args) {

	StringUtil su = new StringUtil();
	byte[] buffer = su.init_start("./License");
	//su.license_15talk_key_decoding(buffer);
	//System.out.println(new String(buffer));
	//System.out.println(new String(buffer));
	//System.out.println(buffer.length);
	//String x = new String(buffer);
	//System.out.println(x.substring(0,x.length()-2));

	//String str = "00147258StringUtil,demo1|StringUtil,demo2|StringUtil,demo3";
	//byte[] buffer = str.getBytes();
	//su.init_callback(buffer);
    }
}


