
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

int lg_string_to_json(struct printbuf *pb, const char *str)
{
    int pos = 0, start_offset = 0;
    unsigned char c;
    do {
	c = str[pos];
	switch(c) {
	    case '\0':
		break;
	    case '\b':
	    case '\n':
	    case '\r':
	    case '\t':
	    case '"':
	    case '\\':
	    case '/':
		if(pos - start_offset > 0)
		    printbuf_memappend(pb, str + start_offset, pos - start_offset);
		if(c == '\b') printbuf_memappend(pb, "\\b", 2);
		else if(c == '\n') printbuf_memappend(pb, "\\n", 2);
		else if(c == '\r') printbuf_memappend(pb, "\\r", 2);
		else if(c == '\t') printbuf_memappend(pb, "\\t", 2);
		else if(c == '"') printbuf_memappend(pb, "\\\"", 2);
		else if(c == '\\') printbuf_memappend(pb, "\\\\", 2);
		else if(c == '/') printbuf_memappend(pb, "\\/", 2);
		start_offset = ++pos;
		break;
	    default:
		if(c < ' ') {
		    if(pos - start_offset > 0)
			printbuf_memappend(pb, str + start_offset, pos - start_offset);
		    sprintbuf(pb, "\\u00%c%c",
			    json_hex_chars[c >> 4],
			    json_hex_chars[c & 0xf]);
		    start_offset = ++pos;
		} else pos++;
	}
    } while(c);

    if(pos - start_offset > 0)
	printbuf_memappend(pb, str + start_offset, pos - start_offset);
    return 0;
}

char *lg_string_to_json_r(char *dest, const char *str)
{

    if (str == NULL) return NULL;

    struct printbuf *json_str = NULL;
    if ((json_str = printbuf_new())) {

	lg_string_to_json(json_str, str);
	strcpy(dest, json_str->buf);
	printbuf_free(json_str);
	return dest;
    }

    return NULL;
}


