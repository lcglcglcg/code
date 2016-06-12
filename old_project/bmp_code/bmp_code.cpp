
#include "bmp_code.h"

bmp_code::bmp_code()
{

    head = bmp_code_font_init();
}

bmp_code::~bmp_code()
{

}

void bmp_code::bmp_code_run(bmp_code_load *bmp)
{

    x = 0;
    y = 0;
    int save_percentage = 0;

    bmp_code_font *temp = NULL;
    for(bmp_code_font *node = head; node; node = node->next) {

	int count = analysis.check(
		node->buffer,
		node->height,
		node->width,
		bmp->buffer,
		bmp->height,
		bmp->width);

	double percentage_temp = ((double)count / (double)node->max) * 100;
	int percentage = percentage_temp;

	if (save_percentage < percentage) {

	    temp = node;
	    save_percentage = percentage;
	    x = analysis.x;
	    y = analysis.y;
	}
    }

    id = temp->id;
    percentage = save_percentage;
    analysis.buffer_merger(
	    temp->buffer,
	    temp->height,
	    temp->width,
	    bmp->buffer,
	    bmp->height,
	    bmp->width,
	    x,
	    y,
	    0);
}

bmp_code_t *bmp_code::bmp_code_from_buff(char *buffer)
{

    int in_width = buffer[18];
    int in_height = buffer[22];
    int in_bit = buffer[28];
    if (in_bit != 24)
	return NULL;


    bmp.load(&buffer[54], in_height, in_width);
    bmp.grey_threshold_sort();
    bmp.binarize();

    for (int i = 0; i < 4; i++) {

	bmp_code_run(&bmp);
	code[i].id = id;
	code[i].percentage = percentage;
	code[i].x = x;
	code[i].y = y;
    }

    for (int i = 0; i < 4; i++) {

	for (int j = i + 1; j < 4; j++) {

	    if (code[i].x > code[j].x) {

		code[4] = code[i];
		code[i] = code[j];
		code[j] = code[4];
	    }
	}
    }

    return code;

}


int main(int argc, char *argv[])
{

    char *path = NULL;
    if (argc == 2)
	path = argv[1];
    else
	path = (char *)"./temp.bmp";

    char buffer[40960];
    FILE *fp = fopen(path, "rb");
    if (!fp)return -1;
    fread(buffer, sizeof(char), 40960, fp);

    bmp_code mycode;
    mycode.bmp_code_from_buff(buffer);

    for (int i = 0; i < 4; i++)
	fprintf(stdout, "%c=%d%%\n", mycode.code[i].id, mycode.code[i].percentage);

    return 0;
}



