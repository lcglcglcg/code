
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmp_code.h"
#include "font_code.h"
#include "parse_code.h"

void parse_code_rotary(
	int **out_buffer, 
	int **in_buffer, 
	int height, 
	int width, 
	int rotate_andle)
{

    int center_x = width / 2;
    int center_y = height / 2;
    double angle = rotate_andle * 3.1415926535 / 180;

    int i, j, x, y;
    for (i = 0; i < height; i++)
	for (j = 0; j < width; j++)
	    out_buffer[i][j] = 0;

    for(y = 0; y < height; y++) {

	for(x = 0; x < width; x++) {

	    int old_x  = (int)(x - center_x) * cos(angle) - (y - center_y) * sin(angle) + center_x;
	    int old_y  = (int)(x - center_x) * sin(angle) + (y - center_y) * cos(angle) + center_y;

	    if (old_x >= 0 && old_x < width && old_y >= 0 && old_y < height)
		out_buffer[y][x] = in_buffer[old_y][old_x];
	}
    }
}

void parse_code_buffer_copy(
	int **out_buffer, int out_height, int out_width,
	int **in_buffer, int in_height, int in_width, 
	int y, int x)
{

    int i, j;
    for (i = 0; i < out_height; i++) {

	for (j = 0; j < out_width; j++) {

	    out_buffer[i][j] = in_buffer[i + y][j + x];
	}
    }
}

void parse_code_buffer_clear(
	char **font_buffer,
	int font_height,
	int font_width,
	char **data_buffer,
	int data_height,
	int data_width,
	int x,
	int y,
	int ch)

{

    int i, j;
    for (i = 0; i < font_height; i++) {

	for (j = 0; j < font_width; j++) {

	    if (font_buffer[i][j] && data_buffer[i + y][j + x])
		data_buffer[i + y][j + x] = ch;
	}
    }
}

void parse_code_font_and_bmp_clean(font_code_t *font, bmp_code_t *bmp, int x, int y)
{

    int i, j;
    for (i = 0; i < font->height; i++) {

	for (j = 0; j < font->width; j++) {

	    if (font->buffer[i][j] && bmp->buffer[i + y][j + x])
		bmp->buffer[i + y][j + x] = 0;
	}
    }
}

int _parse_code_font_and_bmp_check(font_code_t *font, bmp_code_t *bmp, int x, int y)
{

    int i, j, count = 0;
    for (i = 0; i < font->height; i++) {

	for (j = 0; j < font->width; j++) {

	    if (i + y < bmp->height && j + x < bmp->width) {

		if (font->buffer[i][j] && bmp->buffer[i + y][j + x])
		    count++;
	    }
	}
    }

    return count;
}

void parse_code_font_and_bmp_check(font_code_t *font, bmp_code_t *bmp, parse_code_t *parse)
{

    int i, j;
    for (i = 0; i < bmp->height; i++) {

	for (j = 0; j < bmp->width; j++) {

	    int pixel_count = _parse_code_font_and_bmp_check(font, bmp, j, i);
	    if (pixel_count > parse->pixel_count) {

		parse->y = i;
		parse->x = j;
		parse->ch = font->ch;
		parse->pixel_count = pixel_count;
		parse->pixel_pct = pixel_count / font->pixel_max * 100;
		parse->font = font;
	    }
	}
    }
}

void parse_code_run(lg_list_t *font_head, bmp_code_t*bmp, parse_code_t *parse)
{

    parse_code_t parse_temp;
    memset(parse, 0, sizeof(parse_code_t));
    memset(&parse_temp, 0, sizeof(parse_code_t));

    lg_list_t *p = NULL;
    lg_list_for_each(p, font_head) {

	font_code_t *node = (font_code_t *)p;
	parse_code_font_and_bmp_check(node, bmp, &parse_temp);
	// fprintf(stdout, "[%c][%d][%0.2f%%]\n", parse_temp.ch, parse_temp.pixel_count, parse_temp.pixel_pct);

	if (parse_temp.pixel_pct > parse->pixel_pct) {

	    memcpy(parse, &parse_temp, sizeof(parse_code_t));
	}
    }

    parse_code_font_and_bmp_clean(parse->font, bmp, parse->x, parse->y);
    // fprintf(stdout, "[%c][%d][%0.2f%%]\n", parse->ch, parse->pixel_count, parse->pixel_pct);
}

void parse_code_sort(parse_code_t *parse)
{

    int i, j;
    for (i = 0; i < 4; i++) {

	for (j = i + 1; j < 4; j++) {

	    if (parse[i].x > parse[j].x) {

		parse[4] = parse[i];
		parse[i] = parse[j];
		parse[j] = parse[4];
	    }
	}
    }
}

