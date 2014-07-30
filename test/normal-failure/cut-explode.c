#include <cut-2/cut.h>
#include <stdio.h>

void __CUT_BRINGUP__Explode( void )
{
  printf( "__CUT_BRINGUP__Explode() called!\n" );
  ASSERT( 2 == 2, "Making sure environment is correct." );
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
  ASSERT( 1 > 2, "One is less than two." );
}

void __CUT_TAKEDOWN__Explode( void )
{
  printf( "__CUT_TAKEDOWN__Explode() called!\n" );
}

