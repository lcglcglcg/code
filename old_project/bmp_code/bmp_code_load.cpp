
#include "bmp_code_load.h"

bmp_code_load::bmp_code_load()
{

    height = 0;
    width = 0;
    buffer = NULL;

    grey_value_max = 0;
    grey_value_min = 0;
    grey_value_threshold = 0;
    grey_value_existed_count = 0;
}

bmp_code_load::~bmp_code_load()
{ 

    if (!buffer)
	return;

    for (int i = 0; i < height; i++)
	delete []buffer[i];

    delete []buffer;
}

int bmp_code_load::open(const char *path)
{

    int in_width;
    int in_height;
    int in_bit;
    FILE *fp = NULL;

    if (!(fp = fopen(path, "rb")))
	return false;

    fseek(fp, 18, SEEK_SET);
    fread(&in_width, sizeof(char), 4, fp);
    fread(&in_height, sizeof(char), 4, fp);

    fseek(fp, 2, SEEK_CUR);
    fread(&in_bit, sizeof(char), 4, fp);

    if (in_bit != 24) {

	fprintf(stdout, "%s: this bmp bit=%d\n", path, in_bit);
	fclose(fp);
	return false;
    }

    fseek(fp, 22, SEEK_CUR);
    char *in_buff = new char[in_width * in_height * 3];
    memset(in_buff, 0, in_width * in_height * 3);

    long count = 0;
    long skip_size = (4 - in_width * 3 % 4) % 4;//计算每行跳过的字节

    for(int i = 0; i < in_height; i++) {

	for(int j = 0; j < in_width * 3; j++) {

	    if (!fread(&in_buff[count++], 1, 1, fp)) {

		fprintf(stdout, "%s: fread err\n", path);
		fclose(fp);
		delete in_buff;
		return false;
	    }
	}

	fseek(fp, skip_size, SEEK_CUR);
    }

    fclose(fp);
    load(in_buff, in_height, in_width);
    delete in_buff;
    return true;
}

int bmp_code_load::load(char *in_buff, const int in_height, const int in_width)
{

    if (in_width <=  0 || in_height <= 0)
	return false;

    if (buffer) {

	for (int i = 0; i < height; i++)
	    delete []buffer[i];

	delete []buffer;
    }

    width = in_width;
    height  = in_height;
    buffer = new int *[height];
    for(int n = 0; n < height; n++)
	buffer[n] = new int[width];

    for (int n = 0, i = height - 1; i >= 0; i--) {

	for (int j = 0; j < width; j++) {

	    int r = in_buff[n++] & 0xFF;
	    int g = in_buff[n++] & 0xFF;
	    int b = in_buff[n++] & 0xFF;
	    buffer[i][j] = (r>g?r:g)>b?(r>g?r:g):b;
	}
    }

    return true;
}

int bmp_code_load::grey_threshold_sort()
{

    grey_value_max = **buffer;
    grey_value_min = **buffer;

    for (int i = 0; i < 256; i++)
	grey_value_count[i] = 0;

    grey_value_existed_count = 0;
    for (int i = 0; i < height; i++) {

	for (int j = 0; j < width; j++) {

	    if (grey_value_max < buffer[i][j])
		grey_value_max = buffer[i][j];

	    if (grey_value_min > buffer[i][j])
		grey_value_min = buffer[i][j];

	    if (grey_value_existed_check(buffer[i][j]) == true)
		grey_value_count_add(buffer[i][j]);
	    else
		grey_value_member(buffer[i][j]);
	}
    }

    grey_value_existed_sort();


    int	threshold_temp[3];
    threshold_temp[0] = 0;
    threshold_temp[1] = (grey_value_max + grey_value_min)/2;
    threshold_temp[2] = 0;

    while(threshold_temp[1] != threshold_temp[0]) {

	threshold_temp[2] = 0.4 *
	    (sum_data(grey_value_existed[0], threshold_temp[1], grey_value_existed, grey_value_count)
	     / sum_data(grey_value_existed[0], threshold_temp[1], grey_value_count)
	     + sum_data(threshold_temp[1]+1, grey_value_existed[grey_value_existed_count-1], 
		 grey_value_existed, grey_value_count)
	     / sum_data(threshold_temp[1]+1, grey_value_existed[grey_value_existed_count-1], 
		 grey_value_count));

	threshold_temp[0] = threshold_temp[1];
	threshold_temp[1] = threshold_temp[2];
    }

    grey_value_threshold = threshold_temp[1];
    return threshold_temp[1];
} 

double bmp_code_load::sum_data(int start_value, int end_value, int *value_data, int *count_data)
{

    int i = 0;
    int sum = 0;

    while (grey_value_existed[i] < start_value)
	i++;

    while (grey_value_existed[i] <= end_value && i < grey_value_existed_count) {

	sum += value_data[i]*count_data[i];
	i++;
    }

    return sum;
} 

double bmp_code_load::sum_data(int start_value, int end_value, int *count_data)
{ 

    int i = 0;
    int sum = 0;

    while (grey_value_existed[i] < start_value)
	i++;

    while (grey_value_existed[i] <= end_value && i < grey_value_existed_count) {

	sum += count_data[i];
	i++;
    }

    return sum;
} 

bool bmp_code_load::grey_value_existed_check(int data)
{

    if (grey_value_existed_count == 0)
	return false;

    for (int i = 0; i < grey_value_existed_count; i++)
	if (data == grey_value_existed[i])
	    return true;

    return false;
} 

bool bmp_code_load::grey_value_member(int data)
{ 

    grey_value_existed[grey_value_existed_count] = data;
    grey_value_count[grey_value_existed_count]++;
    grey_value_existed_count++;
    return true;
}

bool bmp_code_load::grey_value_count_add(int data)
{

    for (int i = 0; i < grey_value_existed_count; i++)
	if (grey_value_existed[i] == data)
	    grey_value_count[i]++;

    return true;
} 

bool bmp_code_load::grey_value_existed_sort()
{

    int tmp_existed, tmp_count;

    for (int i = 0; i < grey_value_existed_count - 1; i++) {

	for (int j = i+1; j < grey_value_existed_count; j++) {

	    if (grey_value_existed[i] > grey_value_existed[j]) {

		tmp_existed = grey_value_existed[i];
		grey_value_existed[i] = grey_value_existed[j];
		grey_value_existed[j] = tmp_existed;
		tmp_count = grey_value_count[i];
		grey_value_count[i] = grey_value_count[j];
		grey_value_count[j] = tmp_count;
	    } 
	}
    }

    return true;
}

bool bmp_code_load::binarize()
{

    for (int i = 0; i < height; i++) {

	for (int j = 0; j < width; j++) {

	    if (buffer[i][j] > grey_value_threshold)
		buffer[i][j] = 0;
	    else
		buffer[i][j] = '#';
	}
    }

    return true;
}

void bmp_code_load::print()
{

#if 0
    for (int i = 0; i < width; i++)
	fprintf(stdout, "_");
    fprintf(stdout, "\n");
#endif

    for (int i = 0; i < height; i++) {

	for (int j = 0; j < width; j++) {

	    if (buffer[i][j])
		fprintf(stdout, "%c", buffer[i][j]);
	    else
		fprintf(stdout, " ");
	}

	fprintf(stdout, "\n");
	//fprintf(stdout, "|%d\n", i);
    }
}

bool bmp_code_load::delete_noise()
{

    for (int i = 1; i < height-1; i++) {

	for (int j = 1; j < width-1; j++) {

	    if (((buffer[i-1][j-1]
			    != buffer[i][j]) + (buffer[i-1][j] 
				!= buffer[i][j]) + (buffer[i-1][j+1] 
				    != buffer[i][j]) + (buffer[i][j-1] 
					!= buffer[i][j]) + (buffer[i][j+1] 
					    != buffer[i][j]) + (buffer[i+1][j-1] 
						!= buffer[i][j]) + (buffer[i+1][j] 
						    != buffer[i][j]) + (buffer[i+1][j+1] 
							!= buffer[i][j])) >= 7) {
		buffer[i][j] = 0;
	    }
	}
    }

    buffer[0][0] = buffer[0][width-1] = buffer[height-1][0] = buffer[height-1][width-1] = 0;

    //处理除角的边界
    for (int i = 1; i < width-2; i++) {

	if ((buffer[0][i] 
		    != buffer[0][i-1]) + (buffer[0][i] 
			!= buffer[0][i+1]) + (buffer[0][i] 
			    != buffer[1][i]) + (buffer[0][i] 
				!= buffer[1][i-1]) + (buffer[0][i] 
				    != buffer[1][i+1]) >= 4) {
	    buffer[0][i] = 0;
	}
    }

    for (int i = 1; i < height-2; i++) {

	if ((buffer[i][0] 
		    != buffer[i-1][0]) + (buffer[i][0] 
			!= buffer[i+1][0]) + (buffer[i][0] 
			    != buffer[i][1]) + (buffer[i][0] 
				!= buffer[i-1][1]) + (buffer[i][0] 
				    != buffer[i+1][1]) >= 4) {
	    buffer[i][0] = 0;
	}
    }

    for (int i = 1; i < width-2; i++) {

	if ((buffer[height-1][i] 
		    != buffer[height-1][i-1]) + (buffer[height-1][i] 
			!= buffer[height-1][i+1]) + (buffer[height-1][i] 
			    != buffer[height-2][i]) + (buffer[height-1][i] 
				!= buffer[height-2][i-1]) + (buffer[height-1][i] 
				    != buffer[height-2][i+1]) >= 4) {

	    buffer[height-1][i] = 0;
	}
    }

    for (int i = 1; i < height-2; i++) {

	if ((buffer[i][width-1] != 
		    buffer[i-1][width-1]) + (buffer[i][width-1] 
			!= buffer[i+1][width-1]) + (buffer[i][width-1] 
			    != buffer[i][width-2]) + (buffer[i][width-1] 
				!= buffer[i-1][width-2]) + (buffer[i][width-1] 
				    != buffer[i+1][width-2]) >= 4) {

	    buffer[i][width-1] = 0;
	}
    }

    return true;
}




