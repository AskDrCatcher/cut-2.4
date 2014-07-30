/*
 * Sample tests, to check test engine.
 * See also ../all-pass as well.
 *
 * By William Tanksley
 */

#include <cut-2/cut.h>

/* A pointless example -- tests nothing useful, will never fail.
 * This nonetheless shows the basic form of tests. */

void __CUT__sample( void )
{
   ASSERT( 1, "Any non-zero value must always pass an ASSERT." );
}

/* A more complex test, consisting of a bringup, a takedown,
 * and the test body. */

static int example_broughtup = 0;

void __CUT_BRINGUP__example( void )
{
	example_broughtup = 1;
}

void __CUT__example ( void )
{
	/* ASSERT explanations are usually phrased as mandatory statements. */
	ASSERT( 1 == example_broughtup, "We must be initialized by now." );
}

void __CUT_TAKEDOWN__example( void )
{
	/* ASSERTs in takedowns are a very bad idea; takedowns have to be able to
	 * run at any time, including when a test has thrown an exception. */
	example_broughtup = 0;
}

void __CUT__example2( void )
{
	ASSERT( 0 == example_broughtup, "We must be uninitialized by now." );
}

