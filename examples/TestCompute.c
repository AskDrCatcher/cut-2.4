#include <stdio.h>
#include "cut.h"
#include "Compute.h"

void __CUT__Sum(void)
{
    ASSERT(sum(1, 2) == 3, "Check sum");
}
