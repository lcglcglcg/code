
#include "bmp_code_analysis.h"

bmp_code_analysis::bmp_code_analysis()
{

}

bmp_code_analysis::~bmp_code_analysis()
{

}

void bmp_code_analysis::print(int **buffer, int height, int width)
{

    for (int j =  0; j < width; j++)
	fprintf(stdout, "_");
    fprintf(stdout, "\n");

    for (int i = 0; i < height; i++) {

	for (int j =  0; j < width; j++) {

	    if (buffer[i][j] > 0 && buffer[i][j] < 10)
		fprintf(stdout, "%d", buffer[i][j]);
	    else if (buffer[i][j])
		fprintf(stdout, "%c", buffer[i][j]);
	    else	
		fprintf(stdout, " ");
	}

	fprintf(stdout, "|%02d\n", i);
    }
}

void bmp_code_analysis::buffer_print(
	int **buffer, int height, int width,
	int x, int y)
{

    for (int j =  0; j < width; j++)
	fprintf(stdout, "_");
    fprintf(stdout, "\n");

    for (int i = 0; i < height; i++) {

	for (int j = 0; j < width; j++) {

	    if (buffer[i + y][j + x])
		fprintf(stdout, "#");
	    else	
		fprintf(stdout, " ");
	}

	fprintf(stdout, "|%02d\n", i);
    }
}

void bmp_code_analysis::rotary(
	int **out_buffer, 
	int **in_buffer, 
	int height, 
	int width, 
	int rotate_andle)
{

    int center_x = width / 2;
    int center_y = height / 2;
    double angle = rotate_andle * 3.1415926535 / 180;

    for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++)
	    out_buffer[i][j] = 0;

    for(int y = 0; y < height; y++) {

	for(int x = 0; x < width; x++) {

	    int old_x  = (int)(x - center_x) * cos(angle) - (y - center_y) * sin(angle) + center_x;
	    int old_y  = (int)(x - center_x) * sin(angle) + (y - center_y) * cos(angle) + center_y;

	    if (old_x >= 0 && old_x < width && old_y >= 0 && old_y < height)
		out_buffer[y][x] = in_buffer[old_y][old_x];
	}
    }
}

void bmp_code_analysis::buffer_copy(
	int **out_buffer, int out_height, int out_width,
	int **in_buffer, int in_height, int in_width, 
	int y, int x)
{

    for (int i = 0; i < out_height; i++) {

	for (int j = 0; j < out_width; j++) {

	    out_buffer[i][j] = in_buffer[i + y][j + x];
	}
    }
}

void bmp_code_analysis::buffer_merger(
	int **font_buffer,
	int font_height,
	int font_width,
	int **data_buffer,
	int data_height,
	int data_width,
	int x,
	int y,
	int ch)

{

    for (int i = 0; i < font_height; i++) {

	for (int j = 0; j < font_width; j++) {

	    if (font_buffer[i][j] && data_buffer[i + y][j + x])
		data_buffer[i + y][j + x] = ch;
	}
    }
}

int bmp_code_analysis::buffer_check(
	int **font_buffer, int font_height, int font_width,
	int **data_buffer, int data_height, int data_width,
	int y, int x)
{

    int i, j;
    int count = 0;
    for (i = 0; i < font_height; i++) {

	for (j = 0; j < font_width; j++) {

	    if (i + y < data_height && j + x < data_width) {

		if (font_buffer[i][j] && data_buffer[i + y][j + x])
		    count++;
	    }
	}
    }

    return count;
}

int bmp_code_analysis::check(
	int **font_buffer, int font_height, int font_width,
	int **data_buffer, int data_height, int data_width)
{

    count = 0;
    int count_temp = 0;

    for (int i = 0; i < data_height; i++) {

	for (int j = 0; j < data_width; j++) {

	    count_temp = buffer_check(
		    font_buffer, font_height, font_width,
		    data_buffer, data_height, data_width,
		    i, j);

	    if (count < count_temp) {

		y = i;
		x = j;
		count = count_temp;
	    }
	}
    }

    return count;
}


