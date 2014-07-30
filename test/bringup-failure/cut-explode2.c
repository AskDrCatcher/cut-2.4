#include <cut-2/cut.h>
#include <stdio.h>

void __CUT_BRINGUP__Explode2( void )
{
  printf( "__CUT_BRINGUP__Explode2() called!\n" );
  ASSERT( 1 == 2, "1 is not equal to 2." );
}

void __CUT__TestD( void )
{
  ASSERT( 1 == 1, "One should always be equal to one." );
}

void __CUT__TestE( void )
{
  ASSERT( 1 != 0, "One is never zero." );
}

void __CUT__TestF( void )
{
  ASSERT( 1 > 2, "One is less than two." );
}

void __CUT_TAKEDOWN__Explode2( void )
{
  printf( "\nSUCCESS: __CUT_TAKEDOWN__Explode2() is still called!\n" );
}

