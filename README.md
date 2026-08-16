# Scriptorium Inkstone

[![Scriptorium Inkstone CI](https://github.com/ScriptoriumLab/scriptorium-inkstone/actions/workflows/scriptorium-inkstone-ci.yml/badge.svg)](https://github.com/ScriptoriumLab/scriptorium-inkstone/actions/workflows/scriptorium-inkstone-ci.yml)

[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

```
================================================================================================================================================

█████████                      ███             █████                        ███                                █████                 █████    
███▒▒▒▒▒███                    ▒▒▒             ▒▒███                        ▒▒▒                                ▒▒███                 ▒▒███     
▒███    ▒▒▒   ██████  ████████  ████  ████████  ███████    ██████  ████████  ████  █████ ████ █████████████      ▒███         ██████   ▒███████ 
▒▒█████████  ███▒▒███▒▒███▒▒███▒▒███ ▒▒███▒▒███▒▒▒███▒    ███▒▒███▒▒███▒▒███▒▒███ ▒▒███ ▒███ ▒▒███▒▒███▒▒███     ▒███        ▒▒▒▒▒███  ▒███▒▒███
▒▒▒▒▒▒▒▒███▒███ ▒▒▒  ▒███ ▒▒▒  ▒███  ▒███ ▒███  ▒███    ▒███ ▒███ ▒███ ▒▒▒  ▒███  ▒███ ▒███  ▒███ ▒███ ▒███     ▒███         ███████  ▒███ ▒███
███    ▒███▒███  ███ ▒███      ▒███  ▒███ ▒███  ▒███ ███▒███ ▒███ ▒███      ▒███  ▒███ ▒███  ▒███ ▒███ ▒███     ▒███      █ ███▒▒███  ▒███ ▒███
▒▒█████████ ▒▒██████  █████     █████ ▒███████   ▒▒█████ ▒▒██████  █████     █████ ▒▒████████ █████▒███ █████    ███████████▒▒████████ ████████ 
▒▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒  ▒▒▒▒▒     ▒▒▒▒▒  ▒███▒▒▒     ▒▒▒▒▒   ▒▒▒▒▒▒  ▒▒▒▒▒     ▒▒▒▒▒   ▒▒▒▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒ ▒▒▒▒▒    ▒▒▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒▒▒ ▒▒▒▒▒▒▒▒  
                                    ▒███                                                                                                      
                                    █████                                                                                                     
                                    ▒▒▒▒▒                                                                                                      

================================================================================================================================================
```

## 1. Introduction

**Scriptorium Inkstone** is the **Core Logic Server (The Brain)** of the Scriptorium IME ecosystem.

Designed as an **out-of-process** daemon, it handles all the heavy lifting: state management, Pinyin analysis, candidate ranking, and dictionary lookups. By decoupling the logic from the OS client (Brush), Inkstone ensures that complex calculations never block the user's UI thread and provides a crash-resistant architecture.

### Key Features
* **High Performance**: Built with **C++23**, optimized for microsecond-level latency (~17µs per key event).
* **Architecture Agnostic**: Pure C++ logic, free from Windows headers or macOS APIs.
* **Protocol Driven**: Communicates via high-speed IPC (Named Pipes) using a clean JSON protocol.

## 2. Architecture

Inkstone follows a strict **Onion Architecture** (Hexagonal Architecture) to ensure the core logic remains isolated from infrastructure details.

![Scriptorium Inkstone Architecture V1.0](./assets/Scriptorium_Inkstone_Architecture_V1.1.0.png)

### The Layers
1.  **Core Layer (The Domain)**:
    * Contains pure business entities: `PinyinEngine`, `CandidateList`, `InputBuffer`.
    * **Constraint**: No external dependencies (no OS APIs, no IO).
2.  **Manager Layer (The Application)**:
    * Orchestrates the flow. `EngineManager` decides when to query the dictionary and when to commit text.
    * Manages the lifecycle of the input session.
3.  **Infra Layer (The Ports & Adapters)**:
    * **IPC Server**: Implements the Named Pipe server loop.
    * **Service Adapters**: Serializes/Deserializes JSON protocol messages.
    * **Logger**: Asynchronous logging (spdlog).

## 3. Test Strategy

Unlike the Brush (which focuses on Integration), Inkstone's testing strategy prioritizes **Performance** and **Algorithmic Correctness**.

![Scriptorium Inkstone Test Strategy](./assets/Scriptorium_Inkstone_Test_Strategy.png)

### Level 3: Performance Tests (The Crown Jewel)
We use **Google Benchmark** to rigorously track the "speed of light" of our engine.
* **Micro-benchmarks**: Measure the latency of a single key press (IPC + Logic).
* **Big-O Complexity Analysis**: Verify that our algorithms remain $O(N)$ or better under heavy load (e.g., extremely long input strings).
* **RMS Monitoring**: Ensure system stability and low jitter (< 5% RMS).

### Level 2: Integration Tests
* **IPC Protocol Verification**: Ensure the server correctly handles connect/disconnect/reconnect events.
* **End-to-End Logic**: Verify that a sequence of inputs (e.g., "n", "i", "h", "a", "o") produces the correct candidate list via the IPC channel.

### Level 1: Unit Tests
* **Algorithm Testing**: Test specific Pinyin segmentation logic (e.g., splitting "xian" vs "xi'an").
* **Data Structure Testing**: Verify Trie/HashMap lookups and LRU cache behaviors.

## 4. Build & Run

### Prerequisites
* CMake 3.25+
* C++23 Compliant Compiler (MSVC 2022 / Clang 17+ / GCC 13+)
* Ninja (Recommended)

### Building
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Running Benchmarks
```bash
./build/tests/performance_tests/scriptorium_inkstone_performance_tests.exe
```

## 5. Roadmap
The current focus is on implementing the communication protocol and connecting the engine logic.
- [x] Protocol Implementation
  - [x] Integrate `nlohmann/json`.
  - [x] Implement `Service Layer` to bridge IPC strings to Core Structs.
  - [x] Define full JSON schema for `InputEvent` and `CandidateList`.
- [ ] Engine & Logic
  - [x] Connect `PinyinEngine` to the IPC message loop.
  - [x] Implement basic "Exact Match" dictionary lookup.
  - [ ] Implement paging logic for candidates.
- [ ] Infrastructure
  - [ ] Optimize Named Pipe buffer size.
  - [ ] Add support for multiple client connections (future proofing).
  - [ ] Introduce thread pool for handling IPC requests without blocking the main loop.

---

## License

Licensed under the **Apache License 2.0**.

See `LICENSE` for details.

---

*Copyright © 2026 ScriptoriumLab.*
