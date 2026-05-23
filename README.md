*This project has been created as part of the 42 curriculum by jmanani.*

## Description
**Codexion** is a robust, multi-threaded C simulation inspired by Dijkstra's classic Dining Philosophers problem. In this environment, Coders compete for limited shared resources (Dongles) to compile their code. The primary goal is to orchestrate POSIX threads efficiently preventing data races, deadlocks, and coder burnout while supporting both **FIFO** and **EDF (Earliest Deadline First)** real-time scheduling algorithms.

## Instructions
**Compilation:**
Run the following command at the root of the repository:
```bash
make
```

**Execution:**
```bash
./codexion <n_coders> <t_burnout> <t_compile> <t_debug> <t_refactor> <n_compiles> <cooldown> <scheduler>
```
**Examples:**

1. **Standard Simulation (EDF Scheduler):**
5 coders, 800ms burnout, 200ms compile/debug/refactor, each compiles 2 times, 50ms dongle cooldown, using Earliest Deadline First scheduling.
```bash
./codexion 5 800 200 200 200 2 50 edf
```

2. **Standard Simulation (FIFO Scheduler):**
Same parameters as above, but using First In First Out scheduling.
```bash
./codexion 5 800 200 200 200 2 50 fifo
```

3. **Guaranteed Burnout Scenario:**
4 coders, but the burnout time (310ms) is extremely short compared to compile/debug cycle times and dongle contention. One coder will inevitably burn out.
```bash
./codexion 4 310 200 200 200 2 0 fifo
```

## Blocking cases handled

**Deadlock Prevention (Coffman's Conditions):**
Deadlocks are prevented by using a **global FIFO/EDF arbitration queue** that grants both dongles atomically. Coders only acquire dongles when they are top priority and both dongles are available, removing circular wait and hold-and-wait conditions during acquisition.

**Starvation Prevention:**
Access to dongles is strictly managed by a **global** min-heap priority queue.
- Under **EDF**, the priority is based on urgency; the coder closest to burning out is granted access first.
- Under **FIFO**, priority is granted strictly based on the arrival timestamp.

**Cooldown Handling:**
Once a dongle is released, it enters a `cooldown` phase. Coders are coordinated through the global queue and sleep outside the global lock until the cooldown expires, then re-check availability.

**Precise Burnout Detection:**
A dedicated analyzer thread monitors all coders asynchronously. If the time elapsed since a coder's last compile exceeds `t_burnout`, the analyzer safely halts the simulation and logs the burnout within strict tolerance levels.

**Log Serialization:**
All console output is synchronized via a centralized logging mutex. This ensures state messages are strictly serialized and atomic, avoiding messy or interleaved terminal outputs.

## Thread synchronization mechanisms

**`pthread_mutex_t`:**
Mutexes are utilized aggressively to encapsulate state modifications and prevent race conditions.
- State fields (e.g., compile counts, burnout timers) are strictly protected by per-coder mutexes (`coder_mutex`) and centralized mutexes (`cd_mutex`).
- By avoiding inline locking by routing all state modifications through explicit, thread-safe getter and setter functions (`set_bool`, `get_long`, `increase_long`).

**`pthread_cond_t`:**
Condition variables heavily optimize CPU usage.
- Coders wait on the global condition variable when the queue is empty or they are not currently eligible to acquire both dongles.
- Releasing a dongle and enqueueing a request both broadcast on the global condition variable so blocked coders can re-evaluate the queue.

**Safe Execution Wrappers:**
To prevent undefined behavior, raw POSIX calls are routed through a custom Safe Execution Layer (`safe.c`, `safe_cond.c`) using an enum-based architecture (`LOCK`, `UNLOCK`, `BROADCAST`, etc.). If any synchronization primitive fails, the error is caught safely, initiating a graceful thread shutdown without memory leaks.

## Resources
- POSIX Threads standard documentation.
- CodeVault Youtube Playlist - UNix Threads in C (https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
- Youtube about Philosopher (https://www.youtube.com/watch?v=zOpzGHwJ3MU&t=7506s)
- **AI Usage:** AI tools were utilized to discuss architectural edge cases, explore deadlock prevention theories, and generate this documentation. All core algorithms, custom safe wrappers, and asymmetrical locking mechanisms were engineered and implemented by trial, debugging and refinement of logic. Also it helped me lot in code cleaning.
