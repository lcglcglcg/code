public class network {

    static {

	System.loadLibrary("test");
    }

    public native int Network_Init(String ipaddr, int port);
    public native int Network_Connect(String ipaddr, int port);
    public native int Network_Recv(String buffer, int size);
    public native int Network_Send(String buffer, int size);
    public native String Network_String();
    public static void main( String[] args ) {

	int Code;
	network test = new network();

	System.out.println("Network_Init");
	test.Network_Init("0.0.0.0", 8000);
	System.out.println(test.Network_String());

    }
}

