
#include <uuid/uuid.h>
#include "manager_site_builder_module.h"
#include "manager_site_builder_packet.h"
#include "manager_site_builder_io.h"

int manager_site_builder_io_read(char *path, char **buffer, ngx_pool_t *pool)
{

    FILE *fp = fopen(path, "rb");
    if (!fp) return -1;

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (!(*buffer = ngx_pcalloc(pool, size + 1))) {

	fclose(fp);
	return -1;
    }   

    if (fread(*buffer, 1, size, fp) != size) {

	fclose(fp);
	return -1;
    }

    fclose(fp);
    (*buffer)[size] = 0;
    return size;
}

int manager_site_builder_io_write(char *path, char *mode, char *buffer, int size)
{

    FILE *fp = fopen(path, mode);
    if (!fp) return -1;

    if (fwrite(buffer, 1, size, fp) != (size_t)size) {

	fclose(fp);
	return -1;
    }

    if (fwrite("\n", 1, 1, fp) != 1) {

	fclose(fp);
	return -1;
    }

    fclose(fp);
    return 0;
}

int manager_site_builder_io_get_file(char *dir, char *name, char **buffer, ngx_pool_t *pool)
{

    char path[128];
    sprintf(path, "%s/%s/%s", SITE_BUILDER_HOME, dir?dir:"", name?name:"");
    fprintf(stdout, "[get][%s]\n", path);
    return manager_site_builder_io_read(path, buffer, pool);
}

int manager_site_builder_io_add_template_file(char *template_id, char *template_file_name, char *buffer, int size)
{

    char path[128];
    int i, path_size = sprintf(path, "%s/template/%s/%s", SITE_BUILDER_HOME, template_id, template_file_name);
    for (i = 1; i < path_size; i++) {

	if (!strstr(&path[i], "/"))
	    break;

	if (path[i] == '/') {

	    path[i] = 0;
	    if (access(path, 0)) {

		if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
		    return -1;
	    }

	    path[i] = '/';
	}
    }

    // fprintf(stdout, "[add][%s]\n", path);
    return manager_site_builder_io_write(path, "wb", buffer, size);
}

int manager_site_builder_io_add_dict_file(char *dict_id, char *buffer, int size)
{

    char path[128];
    sprintf(path, "%s/dict/%s", SITE_BUILDER_HOME, dict_id);
    
    // fprintf(stdout, "[add][%s]\n", path);
    return manager_site_builder_io_write(path, "a+", buffer, size);
}



