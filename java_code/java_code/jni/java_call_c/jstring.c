//jstring to char*
char* jstringTostring(JNIEnv* env, jstring jstr)
{
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
	rtn = (char*)malloc(alen + 1);
	memcpy(rtn, ba, alen);
	rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

//char* to jstring
jstring stoJstring(JNIEnv* env, const char* pat)
{
    jclass strClass = env->FindClass("Ljava/lang/String;");
    jmethodID ctorID = env->GetMethodID(strClass, "", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

//invoke function
    JNIEXPORT jstring JNICALL Java_test_cs_web_SWIFTAlianceCASmfTest_strcal
(JNIEnv *env, jclass obj, jstring jstr1, jstring jstr2)
{
    jbyteArray bytes = 0;
    jthrowable exc;
    char *pszResult = NULL;
    char *pszSTR1 = NULL;
    char *pszSTR2 = NULL;
    pszSTR1 = jstringTostring(env, jstr1);
    pszSTR2 = jstringTostring(env, jstr2);
    int nlen = sizeof(char)*(strlen(pszSTR1)+strlen(pszSTR2));
    pszResult = (char*)malloc(nlen);
    strcpy(pszResult, pszSTR1);
    strcat(pszResult, pszSTR2);
    jstring jstrRe = stoJstring(env, pszResult);
    free(pszSTR1);
    free(pszSTR2);
    free(pszResult);
    return(jstrRe);
}
