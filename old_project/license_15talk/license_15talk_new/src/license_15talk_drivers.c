
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#ifdef	WIN32
#include <windows.h>
#else
#include <cpuid.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#endif

#ifndef	WIN32
int license_15talk_drivers_mac_addr_ifname(const char *ifname, char *mac_addr)
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

void license_15talk_drivers_mac_addr(char *mac_addr)
{

#ifdef WIN32
    strcpy(mac_addr, "00:00:00:00:00:00");
#else

    int i = 0;
    char *if_name = "eth0";
    if (license_15talk_drivers_mac_addr_ifname(if_name, mac_addr)) {

	struct if_nameindex *nameindex = NULL;
	if (!(nameindex = if_nameindex()))
	    return;

	for(i = 0; nameindex[i].if_index != 0; i++) {

	    if (strcmp(nameindex[i].if_name, "lo") != 0) {

		if_name = nameindex[i].if_name;
		if (license_15talk_drivers_mac_addr_ifname(if_name, mac_addr) == 0) {

		    if_freenameindex(nameindex);
		    return;
		}
	    }
	}

	if_freenameindex(nameindex);
    }
#endif
}

#ifndef	WIN32
void getcpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
{
    __cpuid_count(InfoType, ECXValue, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
}

void getcpuid(unsigned int CPUInfo[4], unsigned int InfoType)
{
    __cpuid(InfoType, CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
}
#endif

void license_15talk_drivers_cpuid(char *cpu_id)
{
#ifdef WIN32
    strcpy(cpu_id, "00000000-00000000-00000000-00000000");
#else
    unsigned int dwBuf[4];
    getcpuidex(dwBuf, 0,0);
    getcpuid(dwBuf, 0);
    sprintf(cpu_id, "%.8X-%.8X-%.8X-%.8X", dwBuf[0],dwBuf[1],dwBuf[2],dwBuf[3]);
#endif
}

int license_15talk_drivers_licence(const char *path, char *licence)
{

    FILE *fp = NULL;
    if (!(fp = fopen(path, "rb")))
	return 0;

    int size = 0;
    if ((size = fread(licence, 1, 4096, fp)) <= 0)
	return 0;

    licence[size] = 0;

    fclose(fp);
    return size;
}




