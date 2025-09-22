#include "ntp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

#define NTP_UNIX_EPOCH_DIFF 2208988800ULL

static double ntp_to_unix_seconds(uint64_t ts)
{
    uint32_t sec = ts >> 32;
    uint32_t frac = ts & 0xFFFFFFFFULL;
    return (double)(sec - NTP_UNIX_EPOCH_DIFF) + frac / (double)(1ULL << 32);
}

int ntp_log_sqlite(const char *dbpath, const ntp_result_t *r);

int ntp_query_once(const char *host, int timeout_ms, ntp_result_t *out)
{
    int sockfd;
    struct addrinfo hints, *server_info;
    struct timespec ts;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(host, "123", &hints, &server_info) != 0)
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

    ntp_packet_t packet = {0};
    packet.li_vn_mode = 0x23;

    clock_gettime(CLOCK_REALTIME, &ts);
    double T1 = ts.tv_sec + ts.tv_nsec / 1e9;

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

    clock_gettime(CLOCK_REALTIME, &ts);
    double T4 = ts.tv_sec + ts.tv_nsec / 1e9;

    double T2 = ntp_to_unix_seconds(__builtin_bswap64(response.recv_ts));
    double T3 = ntp_to_unix_seconds(__builtin_bswap64(response.tx_ts));

    out->delay_s = (T4 - T1) - (T3 - T2);
    out->offset_s = ((T2 - T1) + (T3 - T4)) / 2;

    freeaddrinfo(server_info);
    close(sockfd);

    return 0;
}
