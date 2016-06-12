
#include <uuid/uuid.h>
#include <libxml/parser.h>
#include "manager_site_builder_module.h"
#include "manager_site_builder_packet.h"
#include "manager_site_builder_io.h"

char *lg_uuid(char *key)
{
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid, key);
    return key;
}

void manager_site_builder_packet_code(ngx_http_request_t *r, int code)
{

    char buffer[128] = {0};
    int size = sprintf(buffer, "<code>%d</code>", code);
    return lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

void manager_site_builder_packet_get_conf(ngx_http_request_t *r)
{

    char *buffer = NULL;
    int size = manager_site_builder_io_get_file(NULL, "site_builder_conf.xml", &buffer, r->pool);
    if (size == -1)
	return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_ERR);

    return lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

void manager_site_builder_packet_set_conf_location(xmlNodePtr cur, char *server_name)
{

    char *location_name = (char *)xmlGetProp(cur, (const xmlChar *)"name");
    fprintf(stdout, "\tlocation %s {\n", location_name);

    xmlNodePtr p;
    for (p = cur->children; p; p = p->next) {

	if (strcmp((char *)p->name, "template_id") == 0) {

	    fprintf(stdout, "\t\ttemplate_id %s;\n", (char *)xmlNodeGetContent(p));

	} else if (strcmp((char *)p->name, "keyword_dict") == 0) {

	    fprintf(stdout, "\t\tkeyword_dict %s;\n", (char *)xmlNodeGetContent(p));

	} else if (strcmp((char *)p->name, "link_external_dict") == 0) {

	    fprintf(stdout, "\t\tlink_external_dict %s;\n", (char *)xmlNodeGetContent(p));
	}
    }

    fprintf(stdout, "\t}\n");
}

void manager_site_builder_packet_set_conf_server(xmlNodePtr cur)
{

    fprintf(stdout, "server {\n");
    char *server_name = (char *)xmlGetProp(cur, (const xmlChar *)"name");
    fprintf(stdout, "\tlisten 80;\n");
    fprintf(stdout, "\tserver_name %s;\n", server_name);

    xmlNodePtr p;
    for (p = cur->children; p; p = p->next) {

	if (strcmp((char *)p->name, "location") == 0) {

	    manager_site_builder_packet_set_conf_location(p, server_name);
	}
    }

    fprintf(stdout, "}\n");
}

int manager_site_builder_packet_set_conf(xmlNodePtr root)
{

    fprintf(stdout, 
	    "events {\n"
	    "use epoll;\n"
	    "worker_connections  10240;\n"
	    "}\n"
	    "http {\n"
	   );

    xmlNodePtr p;
    for (p = root->children; p; p = p->next) {

	if (strcmp((char *)p->name, "server") == 0) {

	    manager_site_builder_packet_set_conf_server(p);
	}
    }

    fprintf(stdout, "}\n");
    return 0;
}

void manager_site_builder_packet_create_dict(ngx_http_request_t *r)
{

    char buffer[128], dict_id[40];
    int size = sprintf(buffer, "<dict_id>%s</dict_id>", lg_uuid(dict_id));
    return lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

void manager_site_builder_packet_add_dict(ngx_http_request_t *r, xmlNodePtr root)
{

    char *dict_id = NULL;
    char *dict_buf = NULL;

    xmlNodePtr p;
    for (p = root->children; p; p = p->next) {

	if (strcmp((char *)p->name, "dict_id") == 0) {

	    dict_id = (char *)xmlNodeGetContent(p);

	} else if (strcmp((char *)p->name, "dict_buf") == 0) {

	    dict_buf = (char *)xmlNodeGetContent(p);
	}
    }

    if (!dict_id || !dict_buf)
	return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_FORMAT_ERR);

    if (manager_site_builder_io_add_dict_file(dict_id, dict_buf, strlen(dict_buf)))
	return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_ERR);

    return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_OK);
}

void manager_site_builder_packet_create_template(ngx_http_request_t *r)
{

    char buffer[128], template_id[40];
    int size = sprintf(buffer, "<template_id>%s</template_id>", lg_uuid(template_id));
    return lg_ngx_network_buffer_post_send(r, NULL, buffer, size);
}

void manager_site_builder_packet_add_template(ngx_http_request_t *r, xmlNodePtr root)
{

    char *template_id = NULL;
    char *template_file_name = NULL;
    char *template_buf = NULL;

    xmlNodePtr p;
    for (p = root->children; p; p = p->next) {

	if (strcmp((char *)p->name, "template_id") == 0) {

	    template_id = (char *)xmlNodeGetContent(p);

	} else if (strcmp((char *)p->name, "template_file_name") == 0) {

	    template_file_name = (char *)xmlNodeGetContent(p);

	} else if (strcmp((char *)p->name, "template_buf") == 0) {

	    template_buf = (char *)xmlNodeGetContent(p);
	}
    }

    if (!template_id || !template_file_name || !template_buf)
	return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_FORMAT_ERR);

    if (manager_site_builder_io_add_template_file(template_id, template_file_name, template_buf, strlen(template_buf)))
	return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_ERR);

    return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_OK);
}

void manager_site_builder_packet(ngx_http_request_t *r, char *buffer, int size)
{

    // fprintf(stdout, "[recv][%d][%s]\n", size, buffer);
    xmlDocPtr doc = xmlParseMemory(buffer, size);
    if(doc == NULL) 
	return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_FORMAT_ERR);

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if(root == NULL) {

	xmlFreeDoc(doc);
	return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_FORMAT_ERR);
    }

    char *packet_type = (char *)xmlGetProp(root, (const xmlChar *)"type");
    if (!packet_type) {

	xmlFreeDoc(doc);
	return manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_FORMAT_ERR);
    }

    fprintf(stdout, "[packet_type][%s]\n", packet_type);
    if (strcmp(packet_type, "get_conf") == 0) {

	manager_site_builder_packet_get_conf(r);

    } else if (strcmp(packet_type, "set_conf") == 0) {

	manager_site_builder_packet_code(r, manager_site_builder_packet_set_conf(root));

    } else if (strcmp(packet_type, "create_dict") == 0) {

	manager_site_builder_packet_create_dict(r);

    } else if (strcmp(packet_type, "add_dict") == 0) {

	manager_site_builder_packet_add_dict(r, root);

    } else if (strcmp(packet_type, "create_template") == 0) {

	manager_site_builder_packet_create_template(r);

    } else if (strcmp(packet_type, "add_template") == 0) {

	manager_site_builder_packet_add_template(r, root);

    } else {

	manager_site_builder_packet_code(r, MANAGER_SITE_BUILDER_PACK_FORMAT_ERR);
    }

    // free xml
    xmlFreeDoc(doc);
}





