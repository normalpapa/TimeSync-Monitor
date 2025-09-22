#ifndef NTP_H
#define NTP_H

#include <stdint.h>

typedef struct ntp
{
    uint8_t li_vn_mode;
    uint8_t statum;
    uint8_t poll;
    int8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t ref_id;
    uint64_t ref_ts;
    uint64_t orig_ts;
    uint64_t recv_ts;
    uint64_t tx_ts;
} __attribute__((packed)) ntp_packet_t;

typedef struct
{
    double offset_s;
    double delay_s;
} ntp_result_t;

int ntp_query_once(const char *host, int timeout_ms, ntp_result_t *out);

int ntp_log_sqlite(const char *dbpath, const ntp_result_t *r);

#endif
