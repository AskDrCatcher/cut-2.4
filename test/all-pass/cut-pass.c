#include <cut-2/cut.h>
#include <stdio.h>

void __CUT_BRINGUP__Pass( void )
{
  printf( "__CUT_BRINGUP__Pass() called!\n" );
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
  ASSERT( 2 > 1, "Two is greater than one." );
}

void __CUT_TAKEDOWN__Pass( void )
{
  printf( "__CUT_TAKEDOWN__Pass() called!\n" );
}

