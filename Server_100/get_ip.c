#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <errno.h>

#define ETH_NAME	"ens33"

char *get_ip(void)
{
    int sock;
    struct sockaddr_in sin;
    struct ifreq ifr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return 0;
    }

    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl");
        return 0;
    }

    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    if(inet_ntoa(sin.sin_addr))
        return inet_ntoa(sin.sin_addr);
    return "127.0.0.1";
}
