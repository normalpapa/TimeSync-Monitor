#include "ntp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{
    int sockfd;
    struct addrinfo hints, *server_info;
    const char *host = "pool.ntp.org";
    const char *port = "123";

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(host, port, &hints, &server_info) != 0)
    {
        perror("getaddrinfo");
        return 1;
    }

    sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (sockfd < 0)
    {
        perror("socket");
        return 1;
    }

    ntp_packet_t packet;
    memset(&packet, 0, sizeof(packet));
    packet.li_vn_mode = 0x23;

    // 4. Send packet
    if (sendto(sockfd, &packet, sizeof(packet), 0,
               server_info->ai_addr, server_info->ai_addrlen) < 0)
    {
        perror("sendto");
        return 1;
    }

    ntp_packet_t response;
    socklen_t addr_len = server_info->ai_addrlen;
    int bytes = recvfrom(sockfd, &response, sizeof(response), 0,
                         server_info->ai_addr, &addr_len);
    if (bytes < 0)
    {
        perror("recvfrom");
        return 1;
    }

    printf("Received %d bytes from %s\n", bytes, host);

    freeaddrinfo(server_info);
    close(sockfd);

    return 0;
}
