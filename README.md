# CS441/541 Threads (Bounded Buffer)

## Author(s):

Rohan Hari


## Date:

04/01/26


## Description:

This is the threads project. This project implements the bounded buffer problem as a way to learn how to use Pthreads and semaphores. 

## How to build the software

Because the professor was so generous to us and gave us our very own Makefile, to build the software, all you have to type is "make" and it will compile and create an executable called "bounded buffer" which can be run.

## How to use the software

To run the software, you need to type in "./bounded-buffer <arg1> <arg2> <arg3> <arg4>. 

arg1 - How long to sleep beefore terminating.
arg2 - Number of producer threads.
arg3 - Number of consumer threads.
arg4 - Optional argument to specify your buffer length. Default is 10.

## How the software was tested

I did multiple tests to ensure that the threads were working correctly:

1. Basic Functionality 
    - Tests to verify the program ran with normal/expected inputs. This checked to ensure that threads are created correctly. Verified that the buffer never exceeds capacity and that the program terminates after the given time.

2. Buffer Size Variations
    - Testing different buffer sizes. Tested with small buffer for more cintention, big buffer for less contention, and a very small buffer just to see what would happen with a buffer of 1 (nothing good happens).

3. Thread Count Variations
    - This tests different numbers of producers and consumers. More producers than consumers (vice-versa), equal numbers anad lots of each two.

4. Stress Testing (this was fun)
    - I created scenarios to force race conditions. Using a small buffer with many threads, it forced a huge amount of contention in the program. It did its best though!

5. Edge Cases
    - tested very short runtimes, very long runtimes, single threads, and very large buffer sizes. Hopefully I got most of the edge cases with this.

- for each of these sections, I did multiple runs to ensure that I did not see only what I wanted to see for one or two runs. It was actually fun when the outputs kept getting printed to the screen. Felt like a hacker!

## Known bugs and problem areas

No known bugs or problem areas.
