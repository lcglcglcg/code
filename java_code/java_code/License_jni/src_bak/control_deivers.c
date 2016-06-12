
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#ifdef	WIN32
#include <windows.h>
#else
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#endif

#if ( __i386__ )

void get_deivers_cpuid_asm(uint32_t i, uint32_t *buf)
{

    __asm__ (
	    "    mov    %%ebx, %%esi;  "
	    "    cpuid;                "
	    "    mov    %%eax, (%1);   "
	    "    mov    %%ebx, 4(%1);  "
	    "    mov    %%edx, 8(%1);  "
	    "    mov    %%ecx, 12(%1); "
	    "    mov    %%esi, %%ebx;  "
	    : : "a" (i), "D" (buf) : "ecx", "edx", "esi", "memory" );
}

#else /* __amd64__ */

void get_deivers_cpuid_asm(uint32_t i, uint32_t *buf)
{
    uint32_t  eax, ebx, ecx, edx;

    __asm__ (
	    "cpuid"
	    : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (i) );

    buf[0] = eax;
    buf[1] = ebx;
    buf[2] = edx;
    buf[3] = ecx;
}

#endif

void get_deivers_cpuid_lm(unsigned int var, uint32_t *vx)
{
    int i;
    for(i = 0; i < 3; i++) {

	var = (var >> i);
	vx[i] = var;
    }
}

void get_deivers_cpuid(char *cpu_id_str)
{

    uint32_t cpuinfo[5] = {0};
    get_deivers_cpuid_asm(0, cpuinfo);
    int size = sprintf(cpu_id_str, "%s=", (char *)&cpuinfo[1]);

    uint32_t cpuid[4] = {0};
    uint32_t ax[3] = {0},cx[3] = {0},dx[3] = {0};
    get_deivers_cpuid_asm(1, cpuid);
    get_deivers_cpuid_lm(cpuid[0],ax);
    get_deivers_cpuid_asm(3, cpuid);
    get_deivers_cpuid_lm(cpuid[2],cx);
    get_deivers_cpuid_lm(cpuid[3],dx);
    sprintf(&cpu_id_str[size], "%u%u%u%u%u%u%u%u%u", ax[0], ax[1], ax[2], cx[0], cx[1], cx[2], dx[0], dx[1], dx[2]);
}

void get_deivers_disk_filter(char *name, char *serial_no, char *disk_id)
{

    char *str = NULL;
    for (str = name; *str; str++)
	if (isalnum(*str))
	    *disk_id++ = *str;
    *disk_id++ = '=';
    for (str = serial_no; *str; str++)
	if (isalnum(*str))
	    *disk_id++ = *str;
    *disk_id = 0;
}

void get_deivers_disk(char *disk_id)
{

#ifdef	WIN32
#else
    int fd = 0;
    if ((fd = open("/dev/sda",O_RDONLY)) > 0) {

	struct hd_driveid hd;
	if (!ioctl(fd, HDIO_GET_IDENTITY, &hd)) {

	    char *name = (char *)hd.model;
	    char *serial_no = (char *)hd.serial_no;
	    //sprintf(disk_id, "%.40s=%.20s", model, serial_no);
	    get_deivers_disk_filter(name, serial_no, disk_id);
	    return;
	}
    }
#endif
    sprintf(disk_id, "DISKID=NoSerialNumber");
}

#ifndef	WIN32
int get_deivers_mac_addr_ifname(const char *ifname, char *mac_addr)
{

    int sockfd = -1;
    if ((sockfd = socket( AF_INET, SOCK_STREAM, 0 )) < 0)
	return -1;

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, ifname);
    if (ioctl( sockfd, SIOCGIFHWADDR, &ifr, sizeof(ifr))) {

	close(sockfd);
	return -1;
    }

    unsigned char *str = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    if (str == NULL) {

	close(sockfd);
	return -1;
    }

    sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", str[0], str[1], str[2], str[3], str[4], str[5]);
    close(sockfd);
    return 0;
}
#endif

void get_deivers_mac_addr(char *mac_addr)
{

    strcpy(mac_addr, "00:11:22:33:44:55");
#ifdef	WIN32
#else

    int i = 0;
    char *if_name = "eth0";
    if (get_deivers_mac_addr_ifname(if_name, mac_addr)) {

	struct if_nameindex *nameindex = NULL;
	if (!(nameindex = if_nameindex()))
	    return;

	for(i = 0; nameindex[i].if_index != 0; i++) {

	    if (strcmp(nameindex[i].if_name, "lo") != 0) {

		if_name = nameindex[i].if_name;
		if (get_deivers_mac_addr_ifname(if_name, mac_addr) == 0) {

		    if_freenameindex(nameindex);
		    return;
		}
	    }
	}

	if_freenameindex(nameindex);
    }

#endif
}

int get_deivers_licence(const char *path, char *licence)
{

    FILE *fp = NULL;
    if (!(fp = fopen(path, "rb")))
	return 0;

    int size = 0;
    if ((size = fread(licence, 1, 2048, fp)) <= 0)
	return 0;

    fclose(fp);
    return size;
}




