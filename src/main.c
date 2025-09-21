#include <stdio.h>
#include "ntp.h"

int main()
{
    ntp_result_t result;
    if (ntp_query_once("pool.ntp.org", 1000, &result) == 0)
    {
        printf("Offset: %.3f ms\n", result.offset_s * 1000);
        printf("Delay : %.3f ms\n", result.delay_s * 1000);
    }
    return 0;
}
