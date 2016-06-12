
import java.io.*;

public class demo {

    static {

	System.loadLibrary("bmpcode");
    }

    public native int bmp_code_init();
    public native void bmp_code_free(int bmp_code_obj);

    public native String bmp_code_from_file(int bmp_code_obj, String file_path);
    public native String bmp_code_from_buffer(int bmp_code_obj, byte buffer[], int size);

    public static void main(String[] args) throws IOException {


	String code_str;
	demo temp = new demo();

	//init
	int bmp_code_obj = temp.bmp_code_init();

	File bmp_file = new File("temp.bmp");
	FileInputStream stream = new FileInputStream(bmp_file);
	int size = stream.available();
	byte[] buffer = new byte[size];
	stream.read(buffer);
	stream.close();

	//code from buff 
	code_str = temp.bmp_code_from_buffer(bmp_code_obj, buffer, size);
	System.out.println(code_str);

	//code from file 
	code_str = temp.bmp_code_from_file(bmp_code_obj, "temp.bmp");
	System.out.println(code_str);

	//free
	temp.bmp_code_free(bmp_code_obj);
    }
}




