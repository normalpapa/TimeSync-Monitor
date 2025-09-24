#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "ntp.h"

static int keep_running = 1;

void handle_sigint(int sig)
{
    (void)sig; // unused
    keep_running = 0;
}

int main()
{
    signal(SIGINT, handle_sigint); // catch Ctrl+C
    signal(SIGTERM, handle_sigint);

    printf("TimeSync daemon started. Press Ctrl+C to stop.\n");

    while (keep_running)
    {
        ntp_result_t result;
        if (ntp_query_once("pool.ntp.org", 1000, &result) == 0)
        {
            printf("Offset: %.3f ms | Delay: %.3f ms\n",
                   result.offset_s * 1000,
                   result.delay_s * 1000);

            ntp_log_sqlite("timesync.db", &result);
        }
        else
        {
            printf("NTP query failed\n");
        }

        sleep(10); // wait 10 seconds before next query
    }

    printf("TimeSync daemon stopped.\n");
    return 0;
}
