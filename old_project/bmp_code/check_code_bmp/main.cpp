
#include "jni.h"
#include "bmp_code_font.h"
#include "bmp_code_load.h"
#include "bmp_code_analysis.h"

#ifdef __cplusplus
extern "C" {
#endif

    JNIEXPORT jint JNICALL Java_bmp_arg_bmp_1code_1init
	(JNIEnv *, jobject);

    JNIEXPORT void JNICALL Java_bmp_arg_bmp_1code_1free
	(JNIEnv *, jobject, jint);

    JNIEXPORT jstring JNICALL Java_bmp_arg_bmp_1code_1from_1file
	(JNIEnv *, jobject, jint, jstring);

    JNIEXPORT jstring JNICALL Java_bmp_arg_bmp_1code_1from_1buffer
	(JNIEnv *, jobject, jint, jbyteArray, jint);


#ifdef __cplusplus
}
#endif

typedef struct {

    int id;
    int percentage;
    int x;
    int y;

}bmp_arg_t;

bmp_arg_t bmp_code_call(bmp_code_font *head, bmp_code_load *bmp, bmp_code_analysis *analysis)
{

    int x = 0;
    int y = 0;
    int save_percentage = 0;
    bmp_code_font *temp = NULL;

#if 0
    int percentage_count = 0;
    int percentage_array[4096][6];
    memset(percentage_array, 0, sizeof(int) * 4096 * 6);
#endif

    for(bmp_code_font *node = head; node; node = node->next) {

#if 0
	int *out_buffer[height];
	int out_buffer_data[height][width];
	for (int i = 0; i < height; i++)
	    out_buffer[i] = out_buffer_data[i];

	for (int i = 0; i < 45; i++) {

	    analysis.rotary(out_buffer,
		    height,
		    width,
		    node->buffer,
		    node->height,
		    node->width,
		    i);
	}

	for (int i = 0; i > -45; i--) {

	    analysis.rotary(out_buffer,
		    height,
		    width,
		    node->buffer,
		    node->height,
		    node->width,
		    i);
	}
#endif

	int count = analysis->check(
		node->buffer,
		node->height,
		node->width,
		bmp->buffer,
		bmp->height,
		bmp->width);

	double percentage_temp = ((double)count / (double)node->max) * 100;
	int percentage = percentage_temp;

#if 0
	percentage_array[percentage_count][0] = node->id;
	percentage_array[percentage_count][1] = percentage_temp;
	percentage_array[percentage_count][2] = node->height;
	percentage_array[percentage_count][3] = node->width;
	percentage_array[percentage_count][4] = analysis->x;
	percentage_array[percentage_count][5] = analysis->y;
	percentage_count++;
#endif

	if (save_percentage < percentage) {

	    temp = node;
	    save_percentage = percentage;
	    x = analysis->x;
	    y = analysis->y;
	}
    }

#if 0
    analysis->buffer_print(
	    bmp->buffer,
	    temp->height,
	    temp->width,
	    x,
	    y);
#endif

#if 0

    for (int i = 0; i < 4096; i++) {

	for (int j = i + 1; j < 4096; j++) {

	    if (percentage_array[i][1] < percentage_array[j][1]) {

		for (int n = 0; n < 6; n++) {

		    int p_temp = percentage_array[i][n];
		    percentage_array[i][n] = percentage_array[j][n];
		    percentage_array[j][n] = p_temp;
		}
	    }
	}
    }
#endif

#if 0
    for (int i = 0; i < 10; i++) {

	for (int j = i + 1; j < 10; j++) {

	    int pos = percentage_array[i][4] - percentage_array[j][4];
	    if (pos == 0 || pos == 1 || pos == -1)
		percentage_array[j][4] = percentage_array[j][3];

	    if (percentage_array[i][4] > percentage_array[j][4]) {

		for (int n = 0; n < 6; n++) {

		    int p_temp = percentage_array[i][n];
		    percentage_array[i][n] = percentage_array[j][n];
		    percentage_array[j][n] = p_temp;
		}

	    }
	}
    }
#endif

#if 0
    for (int i = 0; i < 20; i++) {

	analysis->buffer_print(
		bmp->buffer,
		percentage_array[i][2],
		percentage_array[i][3],
		percentage_array[i][4],
		percentage_array[i][5]);

	fprintf(stdout, "%c=%d h=%d w=%d x=%d y=%d\n",
		percentage_array[i][0],
		percentage_array[i][1],
		percentage_array[i][2],
		percentage_array[i][3],
		percentage_array[i][4],
		percentage_array[i][5]);
    }
#endif

    analysis->buffer_merger(
	    temp->buffer,
	    temp->height,
	    temp->width,
	    bmp->buffer,
	    bmp->height,
	    bmp->width,
	    x,
	    y,
	    0);

    bmp_arg_t mydata;
    mydata.id = temp->id;
    mydata.percentage = save_percentage;
    mydata.x = x;
    mydata.y = y;
    return mydata;
}


    JNIEXPORT jint JNICALL Java_bmp_arg_bmp_1code_1init
(JNIEnv *env, jobject obj)
{

    bmp_code_font *list_font = bmp_code_font_init();
    return (int)list_font;
}

    JNIEXPORT void JNICALL Java_bmp_arg_bmp_1code_1free
(JNIEnv *env, jobject obj, jint bmp_obj)
{

    bmp_code_font *list_font = (bmp_code_font *)bmp_obj;
    bmp_code_font_free(list_font);
}

    JNIEXPORT jstring JNICALL Java_bmp_arg_bmp_1code_1from_1file
(JNIEnv *env, jobject obj, jint bmp_obj, jstring path)
{

    const char *path_str = (env)->GetStringUTFChars(path, 0);
    bmp_code_load bmp;
    bmp_code_analysis analysis;
    bmp_code_font *list_font = (bmp_code_font *)bmp_obj;

    if (bmp.open(path_str) == false)
	return (env)->NewStringUTF("bmp file err!");

    bmp.grey_threshold_sort();
    bmp.binarize();
    //bmp.print();

    bmp_arg_t mydata[5];
    for (int i = 0; i < 4; i++){

	mydata[i] = bmp_code_call(list_font, &bmp, &analysis);
    }

    for (int i = 0; i < 4; i++) {

	for (int j = i + 1; j < 4; j++) {

	    if (mydata[i].x > mydata[j].x) {

		mydata[4] = mydata[i];
		mydata[i] = mydata[j];
		mydata[j] = mydata[4];
	    }
	}
    }

    char code_str[5] = {0};
    for (int i = 0; i < 4; i++)
	code_str[i] = mydata[i].id;

    (env)->ReleaseStringUTFChars(path, path_str);
    return (env)->NewStringUTF(code_str);
}

    JNIEXPORT jstring JNICALL Java_bmp_arg_bmp_1code_1from_1buffer
(JNIEnv *env, jobject obj, jint bmp_obj, jbyteArray buffer, jint size)
{

    char *bmp_buff = (char *)(env)->GetByteArrayElements(buffer, 0);
    int in_width = bmp_buff[18];
    int in_height = bmp_buff[22];
    int in_bit = bmp_buff[28];

    if (in_bit != 24)
	return (env)->NewStringUTF("bmp buffer not 24bit err!");

    bmp_code_load bmp;
    bmp_code_analysis analysis;
    bmp_code_font *list_font = (bmp_code_font *)bmp_obj;
    bmp.load(&bmp_buff[54], in_height, in_width);
    bmp.grey_threshold_sort();
    bmp.binarize();
    //bmp.print();

    bmp_arg_t mydata[5];
    for (int i = 0; i < 4; i++){

	mydata[i] = bmp_code_call(list_font, &bmp, &analysis);
    }

    for (int i = 0; i < 4; i++) {

	for (int j = i + 1; j < 4; j++) {

	    if (mydata[i].x > mydata[j].x) {

		mydata[4] = mydata[i];
		mydata[i] = mydata[j];
		mydata[j] = mydata[4];
	    }
	}
    }

    char code_str[5] = {0};
    for (int i = 0; i < 4; i++)
	code_str[i] = mydata[i].id;

    return (env)->NewStringUTF(code_str);
}

int main(int argc, char **argv)
{

    bmp_code_load bmp;
    bmp_code_analysis analysis;
    bmp_code_font *list_font = bmp_code_font_init();

    if (argc < 2)  {

	fprintf(stdout, "Not bmp Img!\n");
	return -1;
    }

    fprintf(stdout, "open: %s\n", argv[1]);

    bmp.open(argv[1]);
    bmp.grey_threshold_sort();
    bmp.binarize();
    bmp.print();

    bmp_arg_t mydata[5];
    for (int i = 0; i < 4; i++)
	mydata[i] = bmp_code_call(list_font, &bmp, &analysis);

    for (int i = 0; i < 4; i++) {

	for (int j = i + 1; j < 4; j++) {

	    if (mydata[i].x > mydata[j].x) {

		mydata[4] = mydata[i];
		mydata[i] = mydata[j];
		mydata[j] = mydata[4];
	    }
	}
    }

    for (int i = 0; i < 4; i++)
	fprintf(stdout, "[%c][%3d%%]\n", mydata[i].id, mydata[i].percentage);

    return 0;
}


