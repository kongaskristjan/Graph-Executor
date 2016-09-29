# Graph-Executor
Graph executor is a (multithreaded) computation scheduler.

Thread pool lets the user submit tasks for asynchronous execution. Graph executor can be viewed as an extension of thread pool, allowing each computational task to take inputs (which may be outputs from other tasks) and output the result of the computation. Graph executor suits well also for completely non-computational tasks, because of straghtforward creation of eg. strands (collection of jobs, which require sequential completion relative to other jobs in this collection, but which have no dependencies to other jobs).

Graph executor is itself a C++ interface class, which allows several implementations. Currently, a single-threaded implementation is complete, which simplifies debugging and testing in either the multithreaded implementation or the programs that use graph executor. A multithreaded implementation is in progress.
