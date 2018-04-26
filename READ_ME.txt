Jacob Spigle
COP4520
TervelQueueAssignment

TervelQueueAssignment.docx details the proof of correctness, efficiency, as well as the experimental evaluation I gathered from testing.

To run the sequential version of my lock-free queue:
// run this command from the /tervel/tests/queue/
make test-seq-lf

//change directory
cd executables/
cd ver_NA_delay_5_lim_5

//run the executable code with parameters
./queue_tervel_lf.x -num_threads 2 2 1 1 0 0

To run the concurrent version of my lock-free queue:
//run this command from the /tervel/tests/queue/
make test-concur-lf

//change directory
cd executables/
cd ver_NA_delay_5_lim_5

//run the executable code with parameters
./queue_tervel_lf.x -num_threads <Number of Threads> <Threads in Group> <dequeueThreads> <enqueueThreads> 0 0

The Wait-Free version of the queue is not fully implemented.
Initial tests were run as above except with the 'make test-seq-wf' and 'make test-concur-wf' commands.