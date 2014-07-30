#include <stdio.h>
#include "cut.h"
#include "Compute.h"

void __CUT__Sum( void )
{
  ASSERT(3 == sum(1,2), "Check sum");
}
