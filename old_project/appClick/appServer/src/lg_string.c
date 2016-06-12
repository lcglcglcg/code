
#include "lg_string.h"

int lg_string_url_encode(char *dest, char *src, size_t size) 
{

    int n;
    int dest_size = 0;
    char hextable[] = "0123456789ABCDEF";
    unsigned char *str = (unsigned char *)src;

    if (src == NULL) return -1;

    for (n = 0; n < size; n++) {

	if (!isalnum(str[n])) {

	    *dest++ = '%';
	    *dest++ = hextable[str[n] >> 4];
	    *dest++ = hextable[str[n] & 0x0F];
	    dest_size += 3;

	} else {

	    *dest++ = str[n];
	    dest_size++;
	}
    }

    *dest = 0;
    return dest_size;
}

int lg_string_url_decode(char *dest, char *src, size_t size) 
{


    char h, l;
    int vh, vl, n;
    int dest_size = 0;

    if (src == NULL || size < 0) return -1;

    for (n = 0; n < size; n++) {

	if (src[n] == '%') {

	    h = toupper(src[n + 1]);
	    l = toupper(src[n + 2]);
	    vh = isalpha(h) ? (10 + (h - 'A')) : (h - '0');
	    vl = isalpha(l) ? (10 + (l - 'A')) : (l - '0');

	    *dest++ = ((vh << 4) + vl);
	    dest_size++;
	    n++;
	    n++;

	} else if (src[n] == '+') {

	    *dest++ = ' ';
	    dest_size++;

	} else {

	    *dest++ = src[n];
	    dest_size++;
	}
    }

    *dest = 0;
    return dest_size;
}

size_t lg_string_utf8_to_gbk(char *in_buff, size_t in_size, char *out_buff, size_t out_size)
{

    char *in = in_buff;
    char *out = out_buff;
    iconv_t cd = iconv_open("GBK", "UTF-8");
    iconv(cd, &in, &in_size, &out, &out_size);
    iconv_close(cd);
    return strnlen(out_buff, out_size);
}

size_t lg_string_gbk_to_utf8(char *in_buff, size_t in_size, char *out_buff, size_t out_size)
{

    char *in = in_buff;
    char *out = out_buff;
    iconv_t cd = iconv_open("UTF-8", "GBK");
    iconv(cd, &in, &in_size, &out, &out_size);
    iconv_close(cd);
    return strnlen(out_buff, out_size);
}


