
#ifndef	__IP_SEARCH_H__
#define	__IP_SEARCH_H__

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <iconv.h>

class IPSearch {

    public:
	IPSearch();
	IPSearch(char *db_path);
	~IPSearch();

	int init(char *db_path);
	int get_location(char *ip, char *location);
	int get_location_utf8(char *ip, char *location, size_t location_size);

    private:
	char *buffer;
	size_t buffer_size;
	uint32_t ip_to_long(char *ip);
	uint32_t search_index(uint32_t ip);
	int _get_location(uint32_t ip, char *location);
};

#endif


