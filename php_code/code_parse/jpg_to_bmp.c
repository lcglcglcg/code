
#include "gd.h"

void jpg_to_bmp()
{
    // arg
    char *path ="img/3117.jpg";
    
    // load img
    FILE *fp = fopen(path, "rb");
    gdImagePtr im = gdImageCreateFromJpeg(fp);

    int size = 0;
    char *buffer = gdImageBmpPtr(im, &size, 0);
}


