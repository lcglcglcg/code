
#ifndef __MATCH_H__
#define __MATCH_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern int match(char *s, char *p);
extern int matchNoCase(char *s, char *p);
extern int matchBody(char *s, char *p, int nocase);

#endif /* MATCH_H */

