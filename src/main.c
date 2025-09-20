#include <stdio.h>
#include "ntp.h"

int main()
{
    ntp_packet_t packet = {0};
    packet.li_vn_mode = 0x23;

    printf("NTP packet size: %lu bytes/n", sizeof(packet));
    printf("li_vn_mpde field: 0x%X/n", packet.li_vn_mode);

    return 0;
}