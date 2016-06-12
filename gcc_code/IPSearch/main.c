
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <iconv.h>

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

#define REDIRECT_TYPE_1 0x01
#define REDIRECT_TYPE_2 0x02

static uint32_t ip2long(const char *ip)
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
static uint32_t search_index(const uint32_t ip, FILE *qqwry_file) 
{
    fprintf(stdout, "ip=%u\n", ip);

    uint32_t index_ip;
    unsigned char head[8];
    unsigned char index_bytes[7];
    fread(head,8,1,qqwry_file);
    uint32_t index_start,index_end,index_mid;
    index_start = (uint32_t)LE_32(&head[0]);
    index_end = (uint32_t)LE_32(&head[4]);
    fprintf(stdout, "index_start=%d index_end=%d\n", index_start, index_end);

    while (1) {

	if ((index_end-index_start) == 7) {
	    break;
	}

	index_mid = index_end / 7 - index_start / 7;

	if (index_mid % 2 == 0) {
	    index_mid = index_mid / 2;
	} else {
	    index_mid = (index_mid + 1) / 2;
	}

	index_mid = index_start + index_mid * 7;
	fseek(qqwry_file, index_mid, SEEK_SET);
	fread(index_bytes, 7, 1, qqwry_file);
	index_ip = (uint32_t)LE_32(&index_bytes[0]);

	if (index_ip == ip) {
	    break;
	} else if (index_ip < ip) {
	    index_start = index_mid;
	} else {
	    index_end = index_mid;
	}
    }
    
    fprintf(stdout, "index_ip=%u ip=%u\n", index_ip, ip);

    if (index_ip > ip) {

	fseek(qqwry_file, index_start, SEEK_SET);
	fread(index_bytes, 7, 1, qqwry_file);
    }

    return (uint32_t)LE_24(&index_bytes[4]);
}

static int readOrJumpRead(char *location, FILE *qqwry_file, const uint32_t data_index) 
{

    unsigned char c;
    unsigned char data_index_bytes[3];
    uint32_t jump_data_index = 0;

    if (data_index) {

	fseek(qqwry_file,data_index,SEEK_SET);
    }

    c = fgetc(qqwry_file);
    switch (c) {

	case REDIRECT_TYPE_2:
	case REDIRECT_TYPE_1:
	    fread(data_index_bytes, 3, 1, qqwry_file);
	    jump_data_index = LE_24(&data_index_bytes[0]);
	    fseek(qqwry_file, jump_data_index, SEEK_SET);
	    break;

	default:
	    location[strlen(location)] = c;
    }
    if (c) {

	while ((c = fgetc(qqwry_file))) {

	    location[strlen(location)] = c;
	}
    }

    return 1;
}

static int is_cz88(const char *str) 
{

    int i;
    int len = strlen(str) - 7;
    for (i = 0; i < len; i++) {

	if (str[i]=='C'
		&& str[i+1]=='Z'
		&& str[i+2]=='8'
		&& str[i+3]=='8'
		&& str[i+4]=='.'
		&& str[i+5]=='N'
		&& str[i+6]=='E'
		&& str[i+7]=='T'
	   ) {

	    return 1;
	}
    }

    return 0;
}

int qqwry_get_location_by_long(char *addr1,char *addr2,const uint32_t ip,FILE *qqwry_file) 
{

    unsigned char c;
    uint32_t data_index;
    uint32_t addr2_offset;
    unsigned char data_index_bytes[3];

    if (!qqwry_file)
	return 0;

    fseek(qqwry_file, 0, SEEK_SET);
    data_index = search_index(ip, qqwry_file);
    fprintf(stdout, "data_index=%d\n", data_index);

    fseek(qqwry_file, data_index + 4, SEEK_SET);
    c = fgetc(qqwry_file);
    fprintf(stdout, "c=%d\n", c);

    if (c == REDIRECT_TYPE_1) {

	fread(data_index_bytes, 3, 1, qqwry_file);
	data_index = LE_24(&data_index_bytes[0]);
	fseek(qqwry_file, data_index, SEEK_SET);
	c = fgetc(qqwry_file);
	data_index -= 4;
    }

    if (c == REDIRECT_TYPE_2) {

	addr2_offset=data_index + 8;
	fread(data_index_bytes, 3, 1, qqwry_file);

	data_index = LE_24(&data_index_bytes[0]);
	fseek(qqwry_file, data_index, SEEK_SET);

	while ((c = fgetc(qqwry_file)))
	    addr1[strlen(addr1)] = c;

	readOrJumpRead(addr2, qqwry_file, addr2_offset);

    } else {

	addr1[strlen(addr1)] = c;
	while ((c = fgetc(qqwry_file)))
	    addr1[strlen(addr1)]=c;

	readOrJumpRead(addr2,qqwry_file,0);
    }

    if (is_cz88(addr1))
	addr1[0]='\0';
    if (is_cz88(addr2))
	addr2[0]='\0';

    return 1;
}

int qqwry_get_location(char *addr1, char *addr2, char *ip, FILE *qqwry_file) 
{

    return qqwry_get_location_by_long(addr1, addr2, ip2long(ip), qqwry_file);
}

void utf8_to_gbk(char *in_buff, size_t in_size, char *out_buff, size_t out_size)
{

    char *in = in_buff;
    char *out = out_buff;
    iconv_t iconv_obj = iconv_open("GBK", "UTF-8");
    iconv(iconv_obj, &in, &in_size, &out, &out_size);
    iconv_close(iconv_obj);
}

void gbk_to_utf8(char *in_buff, size_t in_size, char *out_buff, size_t out_size)
{

    char *in = in_buff;
    char *out = out_buff;
    iconv_t iconv_obj = iconv_open("UTF-8", "GBK");
    iconv(iconv_obj, &in, &in_size, &out, &out_size);
    iconv_close(iconv_obj);
}

int main(int argc, char *argv[])
{

    char addr1[256] = {0};
    char addr2[256] = {0};
    char buffer[1024] = {0};
    char ip[] = "202.97.225.123";
    FILE *fp = fopen("ip.dat", "rw");
    
    qqwry_get_location(addr1, addr2, ip, fp);
    
    gbk_to_utf8(addr1, 256, buffer, 1024);
    fprintf(stdout, "%s\n", buffer);
    
    gbk_to_utf8(addr2, 256, buffer, 1024);
    fprintf(stdout, "%s\n", buffer);
    
    return 0;
}



