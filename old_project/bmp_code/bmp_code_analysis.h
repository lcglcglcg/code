
#ifndef	__bmp_code_analysis_h__
#define	__bmp_code_analysis_h__

#include <stdio.h>
#include <string.h>
#include <math.h>

class bmp_code_analysis
{

    public:
	bmp_code_analysis();
	~bmp_code_analysis();

	void print(int **buffer, int height, int width);
	void buffer_print(
		int **buffer, int height, int width,
		int x, int y);
	void rotary(
		int **out_buffer, 
		int **in_buffer, 
		int height, 
		int width, 
		int rotate_andle);
	void buffer_copy(
		int **out_buffer, int out_height, int out_width,
		int **in_buffer, int in_height, int in_width, 
		int y, int x);
	void buffer_merger(
		int **dest_buffer,
		int dest_height,
		int dest_width,
		int **src_buffer,
		int src_height,
		int src_width,
		int x,
		int y,
		int ch);
	int buffer_check(
		int **dest_buffer, int dest_height, int dest_width,
		int **src_buffer, int src_height, int src_width,
		int y, int x);
	int check(
		int **dest_buffer,
		int dest_height,
		int dest_width,
		int **src_buffer,
		int src_height,
		int src_width);

	int count;
	int x, y;

};

#endif


