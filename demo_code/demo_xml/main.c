
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

void nginx_conf_parse_location(xmlNodePtr cur, char *server_name)
{

    char *location_name = xmlGetProp(cur, "name");

    xmlNodePtr p;
    for (p = cur->children; p; p = p->next) {

	if (strcmp(p->name, "location_name") == 0) {

	    fprintf(stdout, "[%s][%s][%s][%s]\n", server_name, location_name, p->name, (char *)xmlNodeGetContent(p));

	} else if (strcmp(p->name, "template_dir") == 0) {

	    fprintf(stdout, "[%s][%s][%s][%s]\n", server_name, location_name, p->name, (char *)xmlNodeGetContent(p));

	} else if (strcmp(p->name, "keyword_file") == 0) {

	    fprintf(stdout, "[%s][%s][%s][%s]\n", server_name, location_name, p->name, (char *)xmlNodeGetContent(p));

	} else if (strcmp(p->name, "content_file") == 0) {

	    fprintf(stdout, "[%s][%s][%s][%s]\n", server_name, location_name, p->name, (char *)xmlNodeGetContent(p));
	}
    }
}

void nginx_conf_parse_server(xmlNodePtr cur)
{

    char *server_name = xmlGetProp(cur, "name");

    xmlNodePtr p;
    for (p = cur->children; p; p = p->next) {

	if (strcmp(p->name, "location") == 0) {

	    nginx_conf_parse_location(p, server_name);
	}
    }
}

int nginx_conf_parse(char *buffer, int size)
{

    xmlDocPtr doc = xmlParseMemory(buffer, size);
    if(doc == NULL)
	return -1;

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if(root == NULL) {

	xmlFreeDoc(doc);
	return -2;
    }

    xmlNodePtr p;
    for (p = root->children; p; p = p->next) {

	if (strcmp(p->name, "server") == 0) {

	    nginx_conf_parse_server(p);
	}
    }

    xmlFreeDoc(doc);
    return 0;
}

int main() 
{

    FILE *fp = fopen("temp.xml", "r");

    char buffer[4096];
    int size = fread(buffer, 1, 4096, fp);

    nginx_conf_parse(buffer, size);
    return 0;
}


