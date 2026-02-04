                                                  
part 1

A.
Running the producer-consumer code leads to a livelock issue when NUM_PROD_CONS_THREADS
is greater than or equal to 3. It enters an infinite cycle because threads are waiting
on resources that are never available, this is happening because of the mutliple threads
(greater than 2) producer could be going to fast and/or consumer is going to slow. The 
cause of the bug is a failure to correctly handle the semaphore count, which leads to 
blocked threads not being notified when they should be. The note gives an example of this
showing two threads blocked, one thread ready to post and the count goes from -2 to -1 
showing neither can return since allows one thread to proceed, and the second thread is 
left stuck. This is an issue in concurrent programming, where improper synchronization can 
lead to threads being blocked indefinitely.

B.
The bug in parent-child-tasks is the child accessing buffer[idx++] before it's allocated,
causing a SEGFAULT. The bug in the parent-child-tasks.c program is a race condition
between the parent and child threads involving the initialization and access of the shared
buffer. The child thread sets idx = INITIALIZE and length = 100, then proceeds to access
and write to buffer[idx++] before the parent has a chance to allocate memory for buffer
using malloc. Since there is no synchronization to ensure that the parent completes the
buffer allocation before the child accesses it, this results in a segmentation fault when
the child writes to a NULL or uninitialized pointer. The bug occurs because there is no
semaphore protecting  where the buffer is being allocated, allowing the child to proceed
prematurely and access invalid memory.

C.


