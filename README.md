# Graph-Executor
Graph executor is a multithreaded computation scheduler.

Thread pool lets the user submit tasks for asynchronous execution. Graph executor can be viewed as a more complex version of thread pool, allowing each computational task to take inputs, which may be outputs from other tasks (which may or may not be completed yet), thus forming a network of dependencies. Graph executor suits well also for completely non-computational tasks, because of straghtforward creation of non-trivial dependencies, such as strands (collection of jobs, which require sequential completion relative to other jobs in strand). The project also provides several pure thread pool implementations.

Graph executor is itself a C++ interface class, which allows several implementations. Currently, there exists a blocking multithreaded and a single threaded (for debugging and testing) implementation of graph executor. Also, there are lock-free and blocking thread pool implementations, the blocking one being generally faster.
