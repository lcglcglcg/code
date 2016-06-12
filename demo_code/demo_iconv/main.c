
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#define CHECK_CH(ch) ch < 0 ? ch + 256 : ch

int demo_iconv(char *src, size_t src_len, char *dest, size_t dest_len)
{

    size_t size = dest_len;
    iconv_t cd = iconv_open("CSUNICODE", "UTF-8");
    if (cd == (iconv_t)(-1)) return -1;

    if (iconv(cd, &src, &src_len, &dest, &dest_len) == -1) {

	iconv_close(cd);
	return -1;
    }

    iconv_close(cd);
    return size - dest_len;
}

int demo_unicode_print(char *src, int src_len, char *dest, int dest_len)
{

    int i, size = demo_iconv(src, src_len, dest, dest_len);

    for (i = 0; i < size; i++, i++) {

	if (i + 1 <= size) {

	    fprintf(stdout, "&#x%02x%02x;", CHECK_CH(dest[i]), CHECK_CH(dest[i + 1]));
	}
    }

    fprintf(stdout, "\n");
    return 0;
}

int main(int argc, char **argv)
{

    if (argc != 2) {

	fprintf(stderr, "app_utf8_to_unicode [file_path]\n");
	return -1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) return -1;

    char src[409600];
    char dest[409600];
    while(fgets(src, 409600, fp)) {

	int src_len = strlen(src);
	if (--src_len > 0) {

	    if (src[src_len] == '\r') src_len--;

	    src[src_len] = 0;
	    demo_unicode_print(src, src_len, dest, 409600);
	}
    }

    return 0;
}



