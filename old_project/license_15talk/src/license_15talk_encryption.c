
#include <stdio.h>
#include <stdlib.h>

void license_15talk_key_coding(char *buffer, int size)
{

    int i, key;
    for (i = 0; i < size; i++) {

	if (i % 2)
	    buffer[i] = ~buffer[i];

	if (!(i % 3))
	    key = ((i + 1) * 5);
	else if (!(i % 4))
	    key = ((i + 2) / 3);
	else if (i % 5)
	    key = ((i + 3) * 4);
	else
	    key = ((i + 4) / 2);

	if (key % 2)
	    buffer[i] = (buffer[i] + key);
	else
	    buffer[i] = (buffer[i] - key);
    }
}

void license_15talk_key_decoding(char *buffer, int size)
{

    int i, key;
    for (i = 0; i < size; i++) {

	if (!(i % 3)) 
	    key = ((i + 1) * 5);
	else if (!(i % 4))
	    key = ((i + 2) / 3);
	else if (i % 5)
	    key = ((i + 3) * 4);
	else
	    key = ((i + 4) / 2);

	if (key % 2)
	    buffer[i] = (buffer[i] - key);
	else
	    buffer[i] = (buffer[i] + key);

	if (i % 2)
	    buffer[i] = ~buffer[i];
    }
}



