
#include "IPSearch.h"

#define REDIRECT_TYPE_1 0x01
#define REDIRECT_TYPE_2 0x02
#define BE_32(x) ((((uint8_t*)(x))[0]<<24) |\
	(((uint8_t*)(x))[1]<<16) |\
	(((uint8_t*)(x))[2]<<8) |\
	((uint8_t*)(x))[3])
#define LE_32(x) ((((uint8_t*)(x))[3]<<24) |\
	(((uint8_t*)(x))[2]<<16) |\
	(((uint8_t*)(x))[1]<<8) |\
	((uint8_t*)(x))[0])
#define LE_24(x) ((((uint8_t*)(x))[2]<<16) |\
	(((uint8_t*)(x))[1]<<8) |\
	((uint8_t*)(x))[0])

IPSearch::IPSearch()
{
    buffer = NULL;
}

IPSearch::IPSearch(char *db_path)
{

    buffer = NULL;
    init(db_path);
}

IPSearch::~IPSearch()
{

    delete buffer;
}

int IPSearch::init(char *db_path)
{

    if (buffer)	
	return -1;

    FILE *fp = NULL;
    if (!(fp = fopen(db_path, "rd")))
	return -1;

    fseek(fp, 0, SEEK_END);
    if ((buffer_size = ftell(fp)) < 1024)
	return -1;

    if (!(buffer = new char[buffer_size])) {

	fclose(fp);
	return -1;
    }

    fseek(fp, 0, SEEK_SET);
    fread(buffer, buffer_size, 1, fp);
    fclose(fp);
    return 0;
}

uint32_t IPSearch::ip_to_long(char *ip)
{

    uint8_t i, n;
    uint32_t ip_sec = 0;
    int8_t ip_level = 3;
    uint32_t ip_long = 0;
    uint8_t ip_len = strlen(ip);

    for (i = 0;i <= ip_len; i++) {

	if (i != ip_len && ip[i] != '.' && (ip[i] < 48 || ip[i] > 57))
	    continue;

	if (ip[i]=='.' || i == ip_len) {

	    if (ip_level == -1)
		return 0;

	    for (n = 0; n < ip_level; n++)
		ip_sec *= 256;

	    ip_long += ip_sec;
	    if (i == ip_len)
		break;

	    ip_level--;
	    ip_sec = 0;

	} else {

	    ip_sec = ip_sec * 10 + (ip[i] - 48);
	}
    }

    return ip_long;
}

uint32_t IPSearch::search_index(const uint32_t ip)
{

    uint32_t index_ip;
    unsigned char head[8];
    unsigned char index_bytes[7];
    memcpy(head, buffer, 8);

    uint32_t index_start,index_end,index_mid;
    index_start = (uint32_t)LE_32(&head[0]);
    index_end = (uint32_t)LE_32(&head[4]);

    while (1) {

	if ((index_end - index_start) == 7) {
	    break;
	}

	index_mid = index_end / 7 - index_start / 7;

	if (index_mid % 2 == 0) {
	    index_mid = index_mid / 2;
	} else {
	    index_mid = (index_mid + 1) / 2;
	}

	index_mid = index_start + index_mid * 7;
	if (buffer_size < index_mid)
	    return 0;

	memcpy(index_bytes, (buffer + index_mid), 7);
	index_ip = (uint32_t)LE_32(&index_bytes[0]);

	if (index_ip == ip) {
	    break;
	} else if (index_ip < ip) {
	    index_start = index_mid;
	} else {
	    index_end = index_mid;
	}
    }

    if (index_ip > ip) {

	if (buffer_size < index_start)
	    return 0;
	memcpy(index_bytes, (buffer + index_start), 7);
    }

    return (uint32_t)LE_24(&index_bytes[4]);
}

int IPSearch::_get_location(uint32_t ip, char *location)
{

    unsigned char c;
    uint32_t data_index;
    unsigned char data_index_bytes[3];

    data_index = search_index(ip);
    if (data_index == 0)
	return -1;

    c = buffer[data_index + 4];
    if (c == REDIRECT_TYPE_1) {

	if(buffer_size < data_index + 5)
	    return -1;

	memcpy(data_index_bytes, (buffer + data_index + 5), 3);
	data_index = LE_24(&data_index_bytes[0]);
	c = buffer[data_index];
	data_index -= 4;
    }

    if (c == REDIRECT_TYPE_2) {

	if (buffer_size < data_index + 5)
	    return -1;

	memcpy(data_index_bytes, (buffer + data_index + 5), 3);
	data_index = LE_24(&data_index_bytes[0]);
	for (char *str = &buffer[data_index]; *str; str++)
	    location[strlen(location)] = *str;

    } else {

	for (char *str = &buffer[data_index + 4]; *str; str++)
	    location[strlen(location)] = *str;
    }

    return 0;
}

int IPSearch::get_location(char *ip, char *location)
{

    if (buffer == NULL)
	return -1;

    uint32_t ip_addr = 0;
    if (!(ip_addr = ip_to_long(ip)))
	return -2;

    _get_location(ip_addr, location);
    return 0;
}

int IPSearch::get_location_utf8(char *ip, char *location, size_t location_size)
{

    if (buffer == NULL)
	return -1;

    uint32_t ip_addr = 0;
    if (!(ip_addr = ip_to_long(ip)))
	return -2;

    char in_location[256] = {0};
    _get_location(ip_addr, in_location);

    char *in = in_location;
    size_t in_size = strlen(in_location);
    iconv_t iconv_obj = iconv_open("UTF-8", "GBK");
    iconv(iconv_obj, &in, &in_size, &location, &location_size);
    iconv_close(iconv_obj);
    return 0;
}

