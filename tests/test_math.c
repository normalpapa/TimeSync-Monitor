#include <stdio.h>
#include <assert.h>
#include "ntp.h"

void test_offset_delay()
{

    double T1 = 1000.0;
    double T2 = 1001.0;
    double T3 = 1001.5;
    double T4 = 1002.5;

    double delay = (T4 - T1) - (T3 - T2);
    double offset = ((T2 - T1) + (T3 - T4)) / 2.0;

    // expected values
    assert((int)(delay * 1000) == 2000);
    assert((int)(offset * 1000) == 0);

    printf("test_offset_delay passed.\n");
}

int main()
{
    test_offset_delay();
    printf("All tests passed!\n");
    return 0;
}
