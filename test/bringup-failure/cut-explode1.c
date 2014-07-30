#include <cut-2/cut.h>
#include <stdio.h>

void __CUT_BRINGUP__Explode1( void )
{
  fprintf( stderr, "__CUT_BRINGUP__Explode1() called!\n" );
  ASSERT( 1 == 2, "1 is not equal to 2." );
}

void __CUT__TestA( void )
{
  ASSERT( 1 == 1, "One should always be equal to one." );
}

void __CUT__TestB( void )
{
  ASSERT( 1 != 0, "One is never zero." );
}

void __CUT__TestC( void )
{
  ASSERT( 2 > 1, "Two is greater than one." );
}

void __CUT_TAKEDOWN__Explode1( void )
{
  fprintf( stderr, "\nSUCCESS: __CUT_TAKEDOWN__Explode1() is still called!\n" );
}

