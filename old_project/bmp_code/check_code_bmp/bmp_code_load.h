
#ifndef	__bmp_code_load_h__
#define	__bmp_code_load_h__

#include <stdio.h>
#include <string.h>

class bmp_code_load
{
    public:
	bmp_code_load();
	~bmp_code_load();
	int open(const char *path);
	int load(char *in_buffer, int in_height, int in_width);

	int grey_threshold_sort();//计算灰度阈值
	bool binarize();
	void print();
	bool delete_noise();//去噪

	int height;
	int width;
	int **buffer;

	int grey_value_max;
	int grey_value_min;
	int grey_value_threshold;

	int grey_value_count[256];
	bool grey_value_count_add(int);
	bool grey_value_member(int);

	int grey_value_existed[256];
	int grey_value_existed_count;
	bool grey_value_existed_sort();
	bool grey_value_existed_check(int);

	double sum_data(int, int, int *, int *);
	double sum_data(int, int, int *);
};	


#endif


