<!-- This file was hand-converted; man, there has to be a better way of
doing this.  :( -->

<h1>Getting Started with CUT</h2>
<h2>Sergei Gnezdov, 2004 November 11</h2>
<h3>With minor corrections by Samuel A. Falvo II, 2004 November 19</h3>
<hr>
<h2>Introduction</h2>
<p>
Cut is a unit testing framework for C, C++ and Objective-C. This document
covers version 2.3 of cut. Our goal is to help you to get started with cut.
</p>
To download cut go to <a
href="http://www.falvotech.com/projects/cut.php">http://www.falvotech.com/projects/cut.php</a>.

<h2>Getting Ready</h2>
<p>
CUT comes in source code form. You will have to compile it to start using.
You should have no problem to compile it on BSD (FreeBSD) or Linux
machine.
</p>
<p>
To compile run
</p>
<div id="Example"><pre>
  make
</pre></div>
<p>
command without arguments. This should create <tt>cutgen</tt>
application.
</p>
<h2>Test Driven Development in C with CUT</h2>
<h3>Overview</h3>
<p>
To show the usage of the cut we will create a very simple C project. You will have
to create a directory for your project. It is referenced as Project directory further
on. The directory will contain the following files:
</p>
<ul>
<li>Makefile </li>
<li>Main.c </li>
<li>Compute.h </li>
<li>TestCompute.c </li>
<li>Compute.c </li>
<li>cut.h</li>
</ul>
<h3>Creating Makefile</h3>
<p>
Create Makefile with the following content:
</p>
<div id="Example"><pre>
CC = gcc
LD = gcc
MODULES = Compute
OBJS = $(MODULES:%=%.o)
TESTS = $(MODULES:%=Test%.c)
TESTS_OBJS = $(MODULES:%=Test%.o)
LIBS = 
CCOPTS = -c 
DEBUG = 

# default target
help:
	echo "Type 'make application' to build the application."
	echo "Type 'make check' to biuld the application."

# top level target to create production application
application: $(OBJS) Main.o
	$(LD) $(LDOPTS) $(MODULES:%=%.o) Main.o $(LIBS) -o app

# top level target to run test cases
test: cutcheck
	./cutcheck

# top level target to clean up the project directory
clean:
	-rm *.o app cutcheck

cutcheck.c: $(TESTS)
	cutgen -o cutcheck.c $(TESTS)

cutcheck: $(OBJS) $(TESTS_OBJS) cutcheck.o
	$(LD) $(LDOPTS) $(OBJS) $(TESTS_OBJS) $(LIBS)\
            cutcheck.o -o cutcheck

.c.o:
	$(CC) $(CCOPTS) $(DEBUG) -o $@ $<
</pre></div>
<h3>Defining Sum Function</h3>
<p>
Compute.h header defines a function we want to test:
</p>
<div id="Example"><pre>
#ifndef COMPUTE_H_INCLUDED
#define COMPUTE_H_INCLUDED

/* Summarizes two numbers */
int sum(int, int);

#endif
</pre></div>
<h3>Creating Test Case</h3>
<p>
For consistency the test file name is based on the name of the header file beeing
tested. Append Test prefix and replace h with c extension. TestCompute.c file has
the following content:
</p>
<div id="Example"><pre>
#include &lt;stdio.h&gt;
#include "cut.h"
#include "Compute.h"

void __CUT__Sum( void )
{
  ASSERT(3 == sum(1,2), "Check sum");
}
</pre></div>
<p>
TestCompute.c does not compile yet. See it for yourself with the following
command:
</p>
<div id="Example"><pre>
make TestCompute.o
</pre></div>
<p>
The compilation fails because the compiler can&#8217;t find cut.h file. Copy cut.h file
from the cut-2.3 project directory into our project directory.
</p>
<p>
Run
</p>
<div id="Example"><pre>
make TestCompute.o
</pre></div>
<p>
again. This should create TestCompute.o file.
</p>
<h2>Implementing Sum Function</h2>
<p>
Create Compute.c file:
</p>
<div id="Example"><pre>
int sum(int a, int b)
{
  return 0;
}
</pre></div>
<p>
Run
</p>
<div id="Example"><pre>make test</pre></div>
<p>
now. It should fail with the message similar to the following:
</p>
<div id="Example"><pre>
TestCompute.c(7): Check sum
TestCompute.c(7): Failed expression: 3 == sum(1,2)
</pre></div>
<p>
This is because we did not implement sum function yet.
</p>
<p>
Change Compute.c to contain the following code:
</p>
<div id="Example"><pre>
int sum(int a, int b)
{
  return a+b;
}
</pre></div>
<p>
and run <tt>make test</tt> again.  The test should pass now.
</p>
<h3>Creating main application</h3>
<p>
Since our function operates as expected we can create a main application
now:
</p>
<div id="Example"><pre>
#include <stdio.h>
#include "Compute.h"

int main(void)
{
  printf("%i\n", sum(3,2));
  return 0;
}
</pre></div>
<p>
Run <tt>make application</tt> to see the result.

