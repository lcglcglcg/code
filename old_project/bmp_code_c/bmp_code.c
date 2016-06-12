
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_code.h"

int bmp_code_binarize(bmp_code_t *p, char *buffer)
{

    if (p->width <=  0 || p->height <= 0)
	return -1;

    if (p->width >  1024 || p->height > 768)
	return -1;

    int i, j, n;
    for (n = 0, i = p->height - 1; i >= 0; i--) {

	for (j = 0; j < p->width; j++) {

	    int r = buffer[n++] & 0xFF;
	    int g = buffer[n++] & 0xFF;
	    int b = buffer[n++] & 0xFF;
	    int s = (r > g ? r : g) > b ?( r > g ? r : g) : b;
	    p->buffer[i][j] = s > p->grey_value ? 0 : 1;
	}
    }

    return 0;
}

int bmp_code_open(bmp_code_t *p, const char *path)
{

    FILE *fp = NULL;
    if (!(fp = fopen(path, "rb")))
	return -1;

    int width, height, bit;
    fseek(fp, 18, SEEK_SET);
    fread(&width, sizeof(char), 4, fp);
    fread(&height, sizeof(char), 4, fp);
    fseek(fp, 2, SEEK_CUR);
    fread(&bit, sizeof(char), 4, fp);

    fprintf(stdout, "%d:%d:%d\n", width, height, bit);

    if (bit != 24) {

	fclose(fp);
	return -1;
    }

    fseek(fp, 22, SEEK_CUR);
    char *buffer = malloc(width * height * 3);
    memset(buffer, 0, width * height * 3);

    int i, j;
    long count = 0;
    long skip_size = (4 - width * 3 % 4) % 4; //计算每行跳过的字节

    for(i = 0; i < height; i++) {

	for(j = 0; j < width * 3; j++) {

	    if (!fread(&buffer[count++], 1, 1, fp)) {

		fclose(fp);
		free(buffer);
		return -1;
	    }
	}

	fseek(fp, skip_size, SEEK_CUR);
    }

    p->width = width;
    p->height = height;
    int ret = bmp_code_binarize(p, buffer);

    fclose(fp);
    free(buffer);
    return ret;
}

int bmp_code_load(bmp_code_t *p, char *bmp_buffer)
{

    int width = bmp_buffer[18];
    int height = bmp_buffer[22];
    int bit = bmp_buffer[28];
    if (bit != 24)
	return -1;

    char *buffer = malloc(width * height * 3);
    memset(buffer, 0, width * height * 3);

    int i, j;
    long count = 0;
    long skip_size = (4 - width * 3 % 4) % 4; //计算每行跳过的字节
    long buffer_size = 54;
    for(i = 0; i < height; i++) {

	for(j = 0; j < width * 3; j++) {

	    buffer[count++] = bmp_buffer[buffer_size++];
	}

	buffer_size += skip_size;
    }

    p->width = width;
    p->height = height;
    int ret = bmp_code_binarize(p, buffer);

    free(buffer);
    return ret;
}

void bmp_code_print(bmp_code_t *p)
{

    int i, j;
    for (i = 0; i < p->height; i++) {

	for (j = 0; j < p->width; j++) {

	    if (p->buffer[i][j])
		fprintf(stdout, "#");
	    else
		fprintf(stdout, " ");
	}

	fprintf(stdout, "|%d\n", i);
    }
}



