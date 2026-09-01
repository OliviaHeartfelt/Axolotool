## Plugin Manager System (WIP)

* **Modular Architecture:** Plugin-first design enabling runtime feature extension.
* **Dynamic Library Management:** Hot-reloading of shared libraries (.dll / .dylib / .so) without restarting the engine host process.
* **ABI Stability:** Explicit C-compatible interface wrappers for safe cross-binary boundary execution.

> [!IMPORTANT]
> Axolotool v0.1 uses a **mock plugin**.

---

<br/>

## Node Environment System [➥](https://github.com/OliviaHeartfelt/Axolotool/tree/master/src/editor/ANodeEnvironment)

### 1. Registry Subsystem [➥](https://github.com/OliviaHeartfelt/Axolotool/tree/master/src/editor/ANodeEnvironment/Registry)

* **Thread-Safe Concurrent Design:** Safe multi-threaded read/write access across subsystems.
* **Registry Pattern Implementation:**
    * In-memory Database Cache
    * Function & Operation Dispatch Registers
    * Visual Component Registers
* **State Management:** Flyweight pattern for intrinsic node state caching to reduce memory footprint.

<br/>

### 2. Storage Layer Subsystem [➥](https://github.com/OliviaHeartfelt/Axolotool/tree/master/src/editor/ANodeEnvironment/Storage)

* **Database Engine:** Embedded SQLite with Write-Ahead Logging (WAL) mode for concurrent read/write throughput.
* **Schema Design:** 30 relational tables normalized to 4th Normal Form (4NF) with enforced foreign key constraints.
* **Concurrency & Safety:** Thread-safe execution using custom connection pooling and RAII transaction management wrappers.
* **Structural Design:** Facade pattern utilizing composition to decouple underlying SQL queries from domain logic.

<br/>

### 3. Data Streaming & Processing Pipeline Subsystem [➥](https://github.com/OliviaHeartfelt/Axolotool/tree/master/src/editor/ANodeEnvironment/Streaming)

* **Asynchronous Concurrency:** Lock-free / thread-safe Producer-Consumer processing architecture.
* **Bidirectional Synchronization:** rate-limited pipelines bridging storage persistence and render state (DB $\leftrightarrow$ View).
* **Deterministic Resource Management:** Bounded static-size queue preventing unbounded heap allocations and backpressure starvation.
* **C++23 Coroutines:** Zero-overhead lazy sequence evaluation via `std::generator<T>`.

<br/>

### 4. View Layer & GUI Integration Subsystem [➥](https://github.com/OliviaHeartfelt/Axolotool/tree/master/src/editor/ANodeEnvironment/View)

* **Decoupled Render Architecture:** Custom Qt GraphicsView integration separating visual node layout from underlying execution graph logic.
* **Thread Boundary Isolation:** Safe asynchronous signaling between background worker queues and Qt’s main GUI render loop.
* **Structural Design:** Facade pattern wrapping composite scene elements for streamlined UX interactions.

---

<br/>
