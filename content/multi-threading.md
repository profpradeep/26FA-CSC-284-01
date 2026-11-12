# CSC-284: Multithreaded Programming in C++

# 1. Why Multithreading?

## 1 — A Program Doesn't Have to Do Just One Thing at a Time

Consider a program that needs to perform three tasks:

- Download information from a website.
- Process a large dataset.
- Update the user interface.

A traditional sequential program performs these tasks one after another.

```
downloadData();
processData();
updateInterface();
```

The problem is that each operation must finish before the next begins.

Imagine `downloadData()` takes five seconds. The program spends those five seconds waiting, even though other work might be possible.

What if we could have multiple parts of the program working independently?

That's the idea behind multithreading.

## 2 — What Is a Thread?

A thread is an independent path of execution within a process.

Every running program has at least one thread: the main thread.

A multithreaded program creates additional threads that can execute independently of the main thread.

Threads belonging to the same process share:

- The program's global variables.
- Dynamically allocated memory.
- Objects and other process resources.

Each thread has its own:

- Call stack.
- Local variables.
- Execution state.
- Instruction sequence.

The important distinction is that threads execute independently but share the same process memory.

## 3 — Concurrency vs. Parallelism

These two terms are related, but they don't mean exactly the same thing.

Concurrency means multiple tasks can make progress during overlapping periods of time.

Parallelism means multiple tasks are actually executing simultaneously.

Consider a computer with one CPU core:

- The operating system rapidly switches between threads.
- Each thread gets an opportunity to execute.
- They make progress concurrently, but only one executes at a time on that core.

Now consider a computer with multiple CPU cores:

- Different threads can execute on different cores.
- Multiple instructions can execute at the same time.
- This is parallel execution.

![Diagram](https://raw.githubusercontent.com/profpradeep/26FA-CSC-284-01/main/content/images/RevisionDojoConcurrencyVsParallelism.jpg)

[revisiondojo.com](https://www.revisiondojo.com/ib/ib-computer-science/ib-computer-science-4114-concurrent-solution-implementation/notes)

Key takeaway: Multithreading gives us concurrency. Whether that concurrency becomes parallel execution depends on available hardware and scheduling.

## 4 — When Are Threads Useful?

Threads are particularly useful when a program has independent tasks that don't need to wait for one another.

Examples:

| Application      | Background task                                       |
| ---------------- | ----------------------------------------------------- |
| Web browser      | Downloading resources while rendering a page          |
| Video editor     | Encoding video while responding to user input         |
| Game             | Processing physics while handling input               |
| Web scraper      | Retrieving multiple web pages                         |
| Chat application | Receiving network messages while accepting user input |

There are two important motivations:

Responsiveness: A slow operation doesn't prevent the application from doing other work.

Performance: Independent computations can execute in parallel and take advantage of multiple CPU cores.

One important qualification: threads aren't automatically faster. Creating and coordinating threads has overhead. We use them when the benefits outweigh that overhead.

## 5 — A Real-World Example: Web Scraping

Imagine a program that needs to retrieve information from five websites.

A sequential approach looks like this:

[Concurrency vs Parallelism: Significant Differences For Web Scraping](https://images.openai.com/static-rsc-4/6UHnhtgiJI3piaYPNMlvHZqtJRbVIF2ST1eWiTtzLI6KY-Kv-w5H13ijIFuff8hxaRtS4YW6ajg3Sl2nLWP-pH2mwjdF1wfd_z_QqdF2as8NBVKdw5FmzhVadv8aCLkN15_y07tiJLAUHsJwQ1srBaC07o41_bdIXVXFxY6AsP0?purpose=inline)

[proxyscrape.com](https://proxyscrape.com/blog/concurrency-vs-parallelism-significant-differences-for-web-scraping?utm_source=chatgpt.com)

1. Request website A.
2. Wait for its response.
3. Request website B.
4. Wait for its response.
5. Continue until all five websites have responded.

The program spends much of its time waiting for network responses.

Now imagine creating a separate thread for each request.

[Scrapy vs. Beautiful Soup: The 2026 Engineering Benchmark | HasData](https://images.openai.com/static-rsc-4/_7RPjk9ZYAwFeeqvvd0nO8Z4zDds2vQUzutAsze1Oyp_M2WCn32PHrze0lDabVsOXDCN2-YmeISTvuqRgSfqnbtSEHOSGdZCUfLj_jg0D7zxTZ4k5zZRT0OLTGJyMc7oZwNVRAht9PkfUedp27vgLjSESJbN9EU3SKIvdMVhOsM?purpose=inline)

[hasdata.com](https://hasdata.com/blog/scrapy-vs-beautifulsoup?utm_source=chatgpt.com)

Each thread handles a different website.

While one thread waits for a network response, another can continue working.

Discussion: Does this example primarily benefit from multiple CPU cores, or from avoiding time spent waiting?

Answer: Primarily avoiding waiting. This is an example of concurrency benefiting an I/O-bound application.

# 2. Creating Threads in C++

## 6 — Our First Multithreaded Program

C++ provides the `std::thread` class through the `<thread>` header.

Let's start with a simple example. We want five threads to print a message.

```
#include <iostream>
#include <thread>
#include <vector>

void printMessage(int threadNum) {
    std::cout << "Hello from thread #"
              << threadNum << '\n';
}

int main() {
    std::thread t1(printMessage, 1);
    std::thread t2(printMessage, 2);

    t1.join();
    t2.join();

    std::cout << "All threads finished.\n";
}
```

Notice that we haven't changed how `printMessage()` works.

We simply arrange for it to execute on separate threads.

The important new concept is that constructing a `std::thread` starts the thread immediately.

## 7 — What Happens When We Create a Thread?

Look at this statement:

```
std::thread t1(printMessage, 1);
```

Three things happen:

1. A new thread of execution is created.
2. That thread begins executing `printMessage()`.
3. The argument `1` is passed to the function.

The main thread does not wait for `printMessage()` to finish.

It continues executing the next statement.

```
std::thread t1(printMessage, 1);
std::thread t2(printMessage, 2);

std::cout << "Main thread continues.\n";
```

The main thread and the two new threads can now execute concurrently.

Important: We cannot assume which thread will execute first or which will finish first.

## 8 — Understanding Thread IDs

Every thread has an identifier that can be retrieved using:

```
std::this_thread::get_id()
```

The `std::this_thread` namespace provides operations that apply to the thread currently executing the code.

For example:

```
void printMessage(int threadNum) {
    std::cout << "[Thread "
              << std::this_thread::get_id()
              << "] Hello from thread #"
              << threadNum << '\n';
}
```

Here:

- `std::this_thread` refers to the current executing thread.
- `get_id()` returns that thread's unique identifier.
- The identifier helps us distinguish which thread is executing a particular statement.

Other useful operations include:

```
std::this_thread::sleep_for(
    std::chrono::milliseconds(500)
);
```

This pauses the current thread for approximately 500 milliseconds.

We will use this later when working with promises and futures.

## 9 — Creating Multiple Threads

Creating individual thread variables works, but what if we want to create 100 threads?

We don't want to declare 100 variables.

Instead, we can store threads in a container.

```
#include <iostream>
#include <thread>
#include <vector>

void printMessage(int threadNum) {
    std::cout << "[Thread "
              << std::this_thread::get_id()
              << "] Hello from thread #"
              << threadNum << '\n';
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(printMessage, i + 1);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads finished.\n";
}
```

We use a vector because we don't know how many threads we might want to create.

Let's examine the important parts of this program.

## 10 — Why `emplace_back()`?

Consider this statement:

```
threads.emplace_back(printMessage, i + 1);
```

`emplace_back()` constructs a new object directly inside the vector.

In this case, it constructs a `std::thread`.

It forwards the arguments to the thread constructor:

- `printMessage` is the function to execute.
- `i + 1` is the argument passed to that function.

This starts a new thread immediately.

Why not simply copy a thread into the vector?

Because `std::thread` is non-copyable but movable.

A thread represents an active execution resource. Copying it would create ambiguity about which object owns that execution.

`emplace_back()` is convenient because it constructs the thread directly in the container.

## 11 — What Does `join()` Do?

Creating a thread doesn't mean the main thread waits for it.

Consider:

```
std::thread t1(printMessage, 1);

std::cout << "Main thread finished.\n";
```

The main thread may reach the end of `main()` before `t1` finishes.

That creates a serious problem: destroying a joinable `std::thread` causes the program to terminate.

We need a way to wait for the thread.

That's what `join()` does.

```
std::thread t1(printMessage, 1);

t1.join();

std::cout << "Thread finished.\n";
```

When the main thread calls `join()`:

- The main thread pauses.
- The worker thread continues executing.
- Once the worker finishes, `join()` returns.
- The main thread resumes.

Think of `join()` as saying: I started this task, and I need to wait until it is finished before continuing.

## 12 — What Does `joinable()` Do?

A thread is joinable if it represents an active thread of execution that has not yet been joined or detached.

```
if (t.joinable()) {
    t.join();
}
```

`joinable()` returns `true` when the thread can be joined.

It returns `false` if the thread:

- Was default-constructed and never started.
- Has already been joined.
- Has been detached.

It is useful when managing threads whose state might not be known.

In our earlier example, we know that every thread in the vector was successfully started, so we can simply write:

```
for (auto& t : threads) {
    t.join();
}
```

We don't need `joinable()` in that particular example.

## 13 — Understanding the Thread Lifecycle

A thread generally moves through the following stages:

[C++ Threading by Examples — Part 1 | by Anubhav Rohatgi | Medium](https://images.openai.com/static-rsc-4/4H2doZZfzpYJaF9rK8TiCSudCG2ngQ_aisDEb618ChJ-CWq-e-RgEpjy_9g3iBiJran_7yXQpMMdr5cnEaUPRvYPdfHPt802deha7El1kPZhZp67GXQ5HKpL5xP_UESTfQPAZbaMgbCgs2hGMi5Np-gE3zxxV-zJrBNHgPYoYf0?purpose=inline)

[medium.com](https://medium.com/%40anubhavroh/c-threading-primer-c1445595883f?utm_source=chatgpt.com)

1. Created: A `std::thread` object is constructed.
2. Running/ready: The operating system schedules the thread for execution.
3. Completed: The function being executed returns.
4. Joined: Another thread calls `join()` and collects the completed thread's execution.

A thread can also be detached, which allows it to continue independently without being joined.

For this course, we'll focus on joining threads because it gives us predictable control over their lifetime.

Important: A completed thread can still be joinable. Completion of the function and joining the thread are two separate things.

# 3. The Problem With Shared Memory

## 14 — Threads Share Memory

So far, our threads have been independent.

Each thread received its own argument and printed a message.

But what happens when multiple threads need to access the same variable?

Remember:

- Each thread has its own local variables.
- Threads within the same process share global variables and dynamically allocated memory.

That means multiple threads can access and modify the same object.

This is useful because threads can communicate through shared data.

However, it introduces a new problem:

What happens if two threads attempt to modify the same data at the same time?

## 15 — A Simple Example of Shared State

Consider this program:

```
int counter = 0;

void increment() {
    ++counter;
}
```

Suppose two threads call `increment()` simultaneously.

Both threads access the same global variable.

We might expect the counter to increase by two.

But there is a problem.

The statement:

```
++counter;
```

is not necessarily one indivisible operation.

Conceptually, it involves:

1. Reading the current value.
2. Adding one.
3. Writing the new value.

What happens if two threads interleave these operations?

## 16 — How Updates Can Be Lost

Suppose `counter` starts at zero.

Both threads execute `++counter`.

A possible sequence of operations is:

| Step | Thread 1 | Thread 2 | Counter |
| ---- | -------- | -------- | ------- |
| 1    | Reads 0  |          | 0       |
| 2    |          | Reads 0  | 0       |
| 3    | Adds 1   |          | 0       |
| 4    |          | Adds 1   | 0       |
| 5    | Writes 1 |          | 1       |
| 6    |          | Writes 1 | 1       |

We wanted the counter to become 2.

Instead, it becomes 1.

One update was effectively overwritten by the other.

This is called a race condition: the result depends on the timing and ordering of operations.

More specifically, unsynchronized concurrent access involving a write to the same non-atomic object constitutes a data race in C++.

A data race is undefined behavior.

That means the program is not guaranteed to produce any particular result.

## 17 — Let's See the Data Race in Code

```
#include <iostream>
#include <thread>

int counter = 0;

void increment() {
    for (int i = 0; i < 1000; ++i) {
        ++counter;
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Counter: "
              << counter << '\n';
}
```

What should the result be?

Each thread increments the counter 1,000 times.

Expected result:

```
Counter: 2000
```

However, because the threads access the same variable without synchronization, the program contains a data race.

It might print a smaller number, the expected number, or exhibit other behavior.

The important point is not the particular output. The program is incorrect because the shared variable is accessed without proper synchronization.

# 4. Synchronizing Threads

## 18 — How Do We Protect Shared Data?

We need a way to ensure that only one thread at a time modifies a shared resource.

This is called mutual exclusion.

C++ provides `std::mutex` for this purpose.

A mutex acts like a lock:

- A thread acquires the lock before accessing shared data.
- Other threads attempting to acquire the same lock must wait.
- Once the first thread releases the lock, another thread can proceed.

Think of a bathroom with one key.

Only the person holding the key can use the bathroom. Everyone else must wait until the key becomes available.

## 19 — Using `std::mutex`

The simplest approach is to manually lock and unlock a mutex.

```
#include <mutex>

int counter = 0;
std::mutex counterMutex;

void safeIncrement() {
    counterMutex.lock();

    ++counter;

    counterMutex.unlock();
}
```

Let's walk through this:

1. The thread calls `lock()`.
2. If another thread already owns the mutex, it waits.
3. Once the mutex becomes available, the thread acquires it.
4. The thread increments the counter.
5. The thread calls `unlock()`.
6. Another waiting thread can now acquire the mutex.

This ensures that only one thread executes the protected operation at a time.

However, there's a potential problem with manual locking.

What happens if an exception occurs between `lock()` and `unlock()`?

The mutex might never be released.

We need a safer approach.

## 20 — Introducing `std::lock_guard`

C++ provides `std::lock_guard` to manage mutex ownership automatically.

```
void saferIncrement() {
    std::lock_guard<std::mutex> lock(counterMutex);

    ++counter;
}
```

When the `lock_guard` is constructed:

\- It automatically locks the mutex.

When the `lock_guard` goes out of scope:

\- Its destructor automatically unlocks the mutex.

This is an example of RAII (Resource Acquisition Is Initialization).

The resource—in this case, the mutex—is acquired when the object is constructed and released when the object is destroyed.

This is safer than manually managing locks because we don't have to remember to call `unlock()`.

General rule: Prefer RAII-based locking over manually calling `lock()` and `unlock()`.

## 21 — Revisiting Our Counter Example

Let's fix the earlier program.

```
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

int counter = 0;
std::mutex counterMutex;

void increment() {
    for (int i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++counter;
    }
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(increment);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final counter: "
              << counter << '\n';
}
```

Each thread increments the counter 100,000 times.

With ten threads, the expected result is:

```
Final counter: 1000000
```

The mutex ensures that only one thread can increment the counter at a time.

Notice something important:

We haven't eliminated concurrency.

The threads still run independently, but they must take turns when accessing the protected counter.

## 22 — Where Should We Place the Lock?

There is an important performance consideration.

Compare these two implementations.

Approach A: Lock inside the loop

```
void increment() {
    for (int i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++counter;
    }
}
```

The mutex is acquired and released for every increment.

Advantages:

- Other threads can access the counter between increments.
- Each individual modification is protected.

Disadvantages:

- 100,000 lock acquisitions per thread.
- More synchronization overhead.

Approach B: Lock outside the loop

```
void increment() {
    std::lock_guard<std::mutex> lock(counterMutex);

    for (int i = 0; i < 100000; ++i) {
        ++counter;
    }
}
```

The mutex is acquired once for the entire loop.

Advantages:

- Only one lock acquisition per thread.
- Less synchronization overhead.

Disadvantages:

- A thread holds the lock for much longer.
- Other threads cannot access the counter until the entire loop finishes.

Both approaches are correct for this particular example.

The choice depends on how frequently other threads need access to the shared resource.

## 23 — A Subtle Problem: Too Much Locking

We introduced threads to allow work to happen concurrently.

But consider what happens when we put a mutex around almost everything.

```
std::mutex m;

void process() {
    std::lock_guard<std::mutex> lock(m);

    // Thousands of operations...
}
```

If every thread must acquire the same mutex before doing its work, they effectively take turns.

We may lose much of the performance benefit of multithreading.

This introduces an important design principle:

Protect shared data, not entire programs.

Keep critical sections—the portions of code that require exclusive access—as small as practical.

There is a trade-off between synchronization overhead and how much work can happen concurrently.

# 5. The Limitations of Raw Threads

## 24 — We've Learned to Start and Synchronize Threads. What's Missing?

Our examples have been relatively simple:

- Start a thread.
- Perform some work.
- Protect shared variables with a mutex.
- Join the thread.

But imagine a more realistic application.

The main thread starts a background computation and needs the result.

For example:

```
int result = computeSomething();
```

What if `computeSomething()` takes several seconds?

We want it to run in a separate thread, but eventually we need its return value.

This reveals an important limitation of `std::thread`.

## 25 — Problem 1: Returning Values

Consider this:

```
std::thread t([]() {
    return 42;
});
```

The lambda returns 42, but the return value is discarded.

The `std::thread` constructor doesn't provide a mechanism to retrieve the function's return value.

We could use a shared variable:

```
int result = 0;

std::thread t([&]() {
    result = 42;
});

t.join();

std::cout << result << '\n';
```

This works in this simple example because we join the thread before reading the result.

But now consider a more complicated application:

- Multiple threads produce results.
- The main thread needs to know when each result is ready.
- Some results might take longer than others.
- Some computations might fail.

We would need to coordinate access to shared variables and manage synchronization ourselves.

## 26 — Problem 2: Coordinating Work

Suppose the main thread needs to know when a background operation has finished.

One possibility is polling:

```
while (!finished) {
    // Keep checking...
}
```

But this introduces problems:

- The main thread repeatedly checks a variable.
- It may consume CPU time doing nothing useful.
- Accessing `finished` from multiple threads requires synchronization.

Another possibility is to call:

```
std::this_thread::sleep_for(
    std::chrono::seconds(1)
);
```

But sleeping for one second doesn't guarantee the background task will finish in one second.

We need a mechanism that allows one thread to wait until another thread has a result ready.

## 27 — Problem 3: Exception Handling

What happens if a background thread encounters an error?

```
std::thread t([]() {
    throw std::runtime_error("Something went wrong");
});
```

An exception escaping the thread's initial function results in `std::terminate()` being called.

The exception does not automatically propagate to the main thread.

We could catch exceptions inside the worker and store information about them in shared state, but that requires additional coordination.

We would like a more convenient way to transfer both results and exceptions between threads.

## 28 — We Need a Better Communication Mechanism

Let's summarize the limitations we've encountered.

| Problem                                  | What we need              |
| ---------------------------------------- | ------------------------- |
| Cannot retrieve a thread's return value  | A way to transfer results |
| Difficult to know when work is complete  | A way to wait for results |
| Shared variables require synchronization | Safer communication       |
| Exceptions don't propagate automatically | A way to transfer errors  |

C++ provides a mechanism for addressing these problems:

Promises and futures.

Rather than manually managing shared variables and coordinating when results become available, we can use these standard library abstractions.

# 6. Promises and Futures

## 29 — Introducing `std::promise` and `std::future`

C++ provides two classes that allow threads to communicate results:

- `std::promise<T>`
- `std::future<T>`

Think of them as two ends of a communication channel.

[MultiThreading: Providing results - std::promise/future](https://images.openai.com/static-rsc-4/em4o33jSTeS3A6tHEelM5m3UQd1Z7smzQGS1VKZZAMVZinLdb6naYmM8kRgX0OO6dA24OrCw5Urtx3wfPaNLB9MOKFX0d7jeyNzmNTEEMBFvewtdBxhyTlkQdq1TsQTmisJ70800-XfDfvrnenAuRp9NGOahUrTiz5MKazWUH88?purpose=inline)

[gerald-fahrnholz.eu](https://www.gerald-fahrnholz.eu/sw/online_doc_multithreading/html/group___grp_promise_future.html?utm_source=chatgpt.com)

`std::promise<T>` — The producer

A promise is used to provide a value (or an exception) that will become available later.

A thread performing a computation can use:

```
promise.set_value(42);
```

`std::future<T>` — The consumer

A future represents a value that will become available in the future.

Another thread can retrieve that value using:

```
int result = future.get();
```

If the value isn't ready yet, `get()` automatically blocks until it becomes available.

The important distinction:

We no longer need to repeatedly check whether a result is ready. The future handles the waiting for us.

## 30 — How Do Promises and Futures Work Together?

A promise and its associated future share a common state.

The basic workflow is:

1. Create a promise.
2. Obtain the associated future.
3. Give the promise to a worker thread.
4. The worker performs its computation.
5. The worker stores the result using `set_value()`.
6. The receiving thread retrieves the result using `get()`.

Let's look at the individual operations.

Creating the promise:

```
std::promise<int> p;
```

This creates a promise that will eventually provide an integer.

Obtaining the future:

```
std::future<int> f = p.get_future();
```

This obtains the future associated with that promise.

Providing a result:

```
p.set_value(42);
```

This makes the value available.

Retrieving the result:

```
int result = f.get();
```

This waits if necessary and retrieves the value.

An important detail: `get()` can only be called once on a particular `std::future`. It retrieves the result and invalidates that future.

## 31 — A Simple Promise/Future Example

Let's build a complete example.

Imagine we have a worker thread that performs a calculation.

The main thread wants the answer but doesn't want to manage a shared result variable or repeatedly check whether the calculation has finished.

```
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

// Worker function
void worker(std::promise<int> resultPromise) {

    // Simulate some work
    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)
    );

    // Provide the result
    resultPromise.set_value(42);
}

int main() {

    // Create a promise
    std::promise<int> promise;

    // Obtain its associated future
    std::future<int> future = promise.get_future();

    // Start the worker thread
    std::thread t(worker, std::move(promise));

    // Wait for the result
    std::cout << "Main thread: waiting for result...\n";

    int result = future.get();

    std::cout << "Main thread: received result = "
              << result << '\n';

    // Wait for the worker thread to finish
    t.join();

    return 0;
}
```

## 32 — Walking Through the Example

Let's follow what happens when this program executes.

Step 1: Create the promise

```
std::promise<int> promise;
```

We create a promise that will eventually provide an integer.

At this point, no value has been provided.

Step 2: Obtain the future

```
std::future<int> future = promise.get_future();
```

The future is our way of retrieving the result later.

Think of this as establishing the communication channel between the producer and consumer.

Step 3: Start the worker thread

```
std::thread t(worker, std::move(promise));
```

We pass the promise to the worker thread.

Notice the use of `std::move()`.

A promise is non-copyable, so we transfer ownership of it to the worker thread.

The worker now has the promise it needs to provide the result.

## 33 — What Happens While the Worker Executes?

Inside the worker:

```
void worker(std::promise<int> resultPromise) {

    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)
    );

    resultPromise.set_value(42);
}
```

The worker:

1. Waits approximately 500 milliseconds to simulate a time-consuming operation.
2. Calls `set_value(42)`.
3. Makes the result available through the associated future.

Meanwhile, the main thread executes:

```
int result = future.get();
```

Because the worker hasn't provided the result yet, the main thread blocks.

Once the worker calls `set_value(42)`, the future becomes ready.

The main thread resumes and receives the value.

The output is:

```
Main thread: waiting for result...
Main thread: received result = 42
```

Finally, we call:

```
t.join();
```

This ensures that the worker thread has completed before the program continues.

Notice that retrieving the result and joining the thread are separate operations.

## 34 — What If the Worker Encounters an Exception?

Promises and futures can also communicate exceptions.

Instead of providing a value:

```
resultPromise.set_value(42);
```

The worker can provide an exception:

```
resultPromise.set_exception(
    std::make_exception_ptr(
        std::runtime_error("Calculation failed")
    )
);
```

When the receiving thread calls:

```
int result = future.get();
```

The exception is rethrown in the receiving thread.

This allows the main thread to handle the error normally:

```
try {
    int result = future.get();
}
catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << '\n';
}
```

This is a significant improvement over raw threads.

If a promise is destroyed without providing a value or exception, its associated future becomes ready with a `std::future_error` indicating a broken promise.

# 7. Putting It All Together

## 35 — Choosing the Right Tool

We've introduced several mechanisms for working with threads.

Each addresses a different problem.

| Mechanism         | Purpose                               |
| ----------------- | ------------------------------------- |
| `std::thread`     | Create and manage threads             |
| `join()`          | Wait for a thread to finish           |
| `std::mutex`      | Protect shared data                   |
| `std::lock_guard` | Manage mutex locking automatically    |
| `std::promise`    | Provide a result from one thread      |
| `std::future`     | Retrieve a result from another thread |

The important thing is understanding that these tools solve different problems.

- Threads give us independent execution.
- Mutexes help protect shared resources.
- Promises and futures help communicate results.

They can also be used together in the same program.

## 36 — A Different Way to Think About Multithreading

Think about a restaurant.

- The restaurant manager represents the main thread.
- Each cook represents a worker thread.
- The kitchen's shared ingredients represent shared memory.
- A lock on the refrigerator represents a mutex.
- An order ticket represents a promise.
- The completed dish represents the result delivered through a future.

The manager can assign multiple orders to different cooks.

The cooks can work independently, but they must coordinate when accessing shared resources.

When a cook finishes an order, the manager needs a way to know that the result is ready.

That's the general challenge of multithreaded programming: allowing independent work while coordinating access to shared resources and communicating results.

## 37 — Knowledge Check

Question 1: What is the difference between concurrency and parallelism?

Answer: Concurrency means multiple tasks make progress during overlapping periods. Parallelism means multiple tasks execute simultaneously.

Question 2: What happens when we construct a `std::thread`?

Answer: A new thread begins executing the function provided to its constructor.

Question 3: Why do we call `join()`?

Answer: To wait for a thread to finish and ensure its execution is properly completed before its thread object is destroyed.

Question 4: Why is incrementing a shared counter without synchronization dangerous?

Answer: Multiple threads can access and modify the counter concurrently, creating a data race and undefined behavior.

Question 5: What does a mutex accomplish?

Answer: It provides mutual exclusion, allowing only one thread at a time to hold the lock protecting a shared resource.

Question 6: What is the difference between a promise and a future?

Answer: A promise provides a value; a future retrieves that value when it becomes available.

Question 7: Does calling `future.get()` automatically join the worker thread?

Answer: No. It waits for the result to become available, but the thread must still be joined separately.

## 38 — What's Next?

Multithreading introduces a new dimension to programming.

In sequential programming, we generally reason about one execution path at a time.

With multiple threads, we must also consider:

- Which thread executes first?
- What happens when threads access the same data?
- How do we prevent race conditions?
- How do we coordinate independent tasks?
- How do we communicate results and errors?

These questions become increasingly important as applications become more complex.

The techniques we've covered—threads, mutexes, locks, promises, and futures—provide the foundation for writing concurrent C++ programs.
