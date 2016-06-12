public class HelloWorld {
    public native void displayHelloWorld（）；
	public native void printxx（String str）；
	static {
	    System.loadLibrary（"hello"）；
		// System.load（"hello"）；
	}

    public static void main（String[] args） {
	HelloWorld hw = new HelloWorld（）；
	    hw.displayHelloWorld（）；
	    for （int i = 0;; ++i） {
		hw.printxx（"wo kaowo kaowo kaowo kaowo kaowo kaowo kaowo kaowo kaowo kaowo kao"）；
		    if （i % 1000 == 0） {
			try {
			    Thread.sleep（10）；
			} catch （InterruptedException e） {
			}
		    }
	    }
    }
}


//javah HelloWorld
cl -I%java_home%\include -I%java_home%\include\win32 -LDHelloWorld.c -Fehello.dll
#include <jni.h>
#include "HelloWorld.h"
#include <stdio.h>
JNIEXPORT void JNICALL Java_HelloWorld_displayHelloWorld（JNIEnv *env, jobject obj）
{
    printf（"Hello world!\n"）；
	return;
}
JNIEXPORT void JNICALL Java_HelloWorld_printxx
（JNIEnv *env, jobject obj, jstring prompt）{

    const char *str = （*env）->GetStringUTFChars（env, prompt, 0）；
	printf（"%s",prompt）；
	（*env）->ReleaseStringUTFChars（env, prompt, str）；

	return;
}
