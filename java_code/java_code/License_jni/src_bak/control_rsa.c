
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

char rsa_public_key[] = "-----BEGIN PUBLIC KEY-----\r\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCwe+k8B+WQOAV3RSYzQTGOLKXZ\r\nkQxNjjUtIDW+oSPXZg63RfRcMk1gVUzd33x5T4rfwlbZ8yNWfFhHVJyaP7jQm8eI\r\nozTFT6Om3VY1YNghe1n/Drq1/hjzRa8dDNrZTi3FeNn44WygqVzRjt0fwevy2eoR\r\nIbr9tkWui5muFbE+VwIDAQAB\r\n-----END PUBLIC KEY-----";

char rsa_private_key[] = "-----BEGIN RSA PRIVATE KEY-----\r\nMIICXQIBAAKBgQCwe+k8B+WQOAV3RSYzQTGOLKXZkQxNjjUtIDW+oSPXZg63RfRc\r\nMk1gVUzd33x5T4rfwlbZ8yNWfFhHVJyaP7jQm8eIozTFT6Om3VY1YNghe1n/Drq1\r\n/hjzRa8dDNrZTi3FeNn44WygqVzRjt0fwevy2eoRIbr9tkWui5muFbE+VwIDAQAB\r\nAoGBAKxpkXTVHh8jq6tWgbhG83u7AJepTYSi7i5Urky6rgIrsSq5C73aYp8/FiYX\r\nxEnbPIfXGvQ3xQkj1zZwYcGLiSea00ei/p8aWwOieUDJApFHU9DBZMsfaAJXlDZx\r\nccEapExJVL7ePnx/pFiRNoNzNlyZwtm2bo3QWAd5xzDlJmU5AkEA5WoQBYdbgFSu\r\nAdpfYxd7mFfRVNeSU0F6JI91nryYAVw59B6LfGx5qY4Lw+3J5mSWQA5HYDg7n4p1\r\n1bxQKVJFPQJBAMTvlpZNoV5BlNSaVb+OL8HfggEeA6aruSduU8Oi0CeZdpN+hryW\r\nxkfP7GAatCt3XmROP99k33Mhwhj2nUTrUyMCQDFnKrYWFQVItwQdI4dySiJfXuvX\r\nVGJrakMvRZP+Zvl8A9rnLQCJV5XEyhHSRjVAGf8HVCWLVoOfzIpPeC+90L0CQArQ\r\nmXVxl+4ELfcq7pCRb1/3NT14d3zcq1NWBLWWqFW0YMWiyiVrJkJlUbbJYKGOcZmS\r\njoFnGbpJ3U3fpS1Wuv8CQQCzyfFIti0rY0SEeIGpUR4VaPLxliqmNQJAJW0Vlytp\r\nf+SvO9WEEjXd421gSJWfp/EG0tZvjD/N2pir04D1GGB6\r\n-----END RSA PRIVATE KEY-----";

FILE *rsa_fp_from_buffer(char *buffer)
{

    FILE *fp = NULL;
    if (!(fp = fopen("./rsa_temp", "w")))
	return NULL;

    size_t size = strlen(buffer);
    if (fwrite(buffer, 1, size, fp) != size) {

	fclose(fp);
	return NULL;
    }

    fclose(fp);
    if (!(fp = fopen("./rsa_temp", "r")))
	return NULL;

    unlink("./rsa_temp");
    return fp;
}

char *rsa_public_encrypt(char *dest, char *src)
{

    FILE *fp = NULL;
    if (!(fp = rsa_fp_from_buffer(rsa_public_key)))
	return NULL;

    RSA *rsa = NULL;
    if (!(rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL)))
	return NULL;

    if (RSA_public_encrypt(RSA_size(rsa), 
		(unsigned char *)src, 
		(unsigned char *)dest, 
		rsa, RSA_NO_PADDING) < 0)
	return NULL;

    RSA_free(rsa);
    fclose(fp);
    return dest;
}

char *rsa_private_encrypt(char *dest, char *src)
{

    FILE *fp = NULL;
    if (!(fp = rsa_fp_from_buffer(rsa_private_key)))
	return NULL;

    RSA *rsa = NULL;
    if (!(rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL)))
	return NULL;

    if (RSA_private_encrypt(RSA_size(rsa), 
		(unsigned char *)src, 
		(unsigned char *)dest, 
		rsa, RSA_NO_PADDING) < 0)
	return NULL;

    RSA_free(rsa);
    fclose(fp);
    return dest;
}

char *rsa_public_decrypt(char *dest, char *src)
{

    FILE *fp = NULL;
    if (!(fp = rsa_fp_from_buffer(rsa_public_key)))
	return NULL;

    RSA *rsa = NULL;
    if (!(rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL)))
	return NULL;

    if (RSA_public_decrypt(RSA_size(rsa), 
		(unsigned char *)src, 
		(unsigned char *)dest, 
		rsa, RSA_NO_PADDING) < 0)
	return NULL;

    RSA_free(rsa);
    fclose(fp);
    return dest;
}

char *rsa_private_decrypt(char *dest, char *src)
{

    FILE *fp = NULL;
    if (!(fp = rsa_fp_from_buffer(rsa_private_key)))
	return NULL;

    RSA *rsa = NULL;
    if (!(rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL)))
	return NULL;

    if (RSA_private_decrypt(RSA_size(rsa), 
		(unsigned char *)src, 
		(unsigned char *)dest, 
		rsa, RSA_NO_PADDING) < 0)
	return NULL;

    RSA_free(rsa);
    fclose(fp);
    return dest;
}

int rsa_main(void)
{

    char str1[1024] = "abc123";
    char str2[1024] = {0};
    char str3[1024] = {0};

#if 1
    rsa_public_encrypt(str2, str1);
    rsa_private_decrypt(str3, str2);
#else
    rsa_private_encrypt(str2, str1);
    rsa_public_decrypt(str3, str2);
#endif

    fprintf(stdout, "%s\n", str3);
    return 0;
}


