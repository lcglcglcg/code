public class demo {

    static {

	System.loadLibrary("demo");
    }

    public native void SetString(String str);
    public native String GetString();

    public native byte[] SetBytes(byte buffer[], int len);

    public static void main(String[] args) {

	demo temp = new demo();

	temp.SetString("中国");
	System.out.println(temp.GetString());

	ByteArrayOutputStream baos = new ByteArrayOutputStream();
	//SetBytes("美国", 10);
    }
}

