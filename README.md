<!-- Banner -->
<p align="center">
  <img src=".github/assets/axolotool_banner.svg" alt="Axolotool Banner" width="100%">
</p>

<br/>

<!-- Badges -->
<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue?logo=cplusplus" alt="C++23">
  <img src="https://img.shields.io/badge/Qt-6.11-3fb950?logo=qt&logoColor=white" alt="Qt6.11">
  <img src="https://img.shields.io/badge/License-MPL--2.0-blue" alt="License">
  <img src="https://img.shields.io/badge/Version-v0.1-f0883e" alt="Version">
  <img src="https://img.shields.io/badge/Lines%20of%20code-13.2k-6f42c1" alt="Lines of Code">
</p>


## 🚀 Introduction

**Axolotool** is a native (WIP) **Live-App Builder** for real-time desktop applications. 

Instead of compiling and running separate binaries, you build and execute your application live inside Axolotool. 
You can make your own applications using the built-in node editor with standard plugins, or create custom plugins to add new features.

<br/>

> [!TIP]
> 🎬 **Watch the Demo:** [Axolotool v0.1 demo](https://drive.google.com/file/d/1avH0CEg7C-EfxB9lbyyiUPOWbxltmhvb/view?usp=drive_link)

<br/>

<!-- Table of Contents -->
## 📑 Table of Contents

* 🛠️ [Tech Stack](#%EF%B8%8F-tech-stack)
* 🧩 [Main Systems](#-main-systems)
* 📁 [Project Structure](#-project-structure)
* 📦 [Architecture](#-architecture)
* ⚡ [Features](#-features)
* 🤝 [Contributing](#-contributing)
* 🎯 [What's Next?](#-whats-next)
* 🤖 [AI-Assisted Development](#-ai-assisted-development)
* ⚖️ [License & Compliance](#%EF%B8%8F-license--compliance)
* 👩‍💻 [Author](#%E2%80%8D-author)

<br/>

## 🛠️ Tech Stack

<table>
  <thead>
    <tr>
      <th width="170">Category</th>
      <th width="160">Technology</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><b>Language</b></td>
      <td>C++23</td>
    </tr>
    <tr>
      <td><b>UI Framework</b></td>
      <td>Qt 6.11</td>
    </tr>
    <tr>
      <td><b>Database</b></td>
      <td>SQLite</td>
    </tr>
    <tr>
      <td><b>Build System</b></td>
      <td>CMake 3.28</td>
    </tr>
  </tbody>
</table>

---

<br/>

## 🧩 Main Systems

<table>
  <thead>
    <tr> 
      <th width="170">Main System</th> 
      <th width="90" align="center">Version</th> 
      <th>Description</th> 
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td><b>Node Environment</b></td> 
      <td align="center">$\textsf{\color{#50E650}{v1.0}}$</td> 
      <td>Visual workspace and interactive editor interface for designing node workflows.</td> 
    </tr>
    <tr> 
      <td><b>Plugin Manager</b></td>   
      <td align="center">$\textsf{\color{#E62E2E}{v0.0}}$</td> 
      <td>Dynamic loader and lifecycle manager for cross-platform shared libraries (.dll&nbsp;/&nbsp;.dylib&nbsp;/&nbsp;.so).</td> 
    </tr>
    <tr> 
      <td><b>Execution Graph</b></td>  
      <td align="center">$\textsf{\color{#E62E2E}{v0.0}}$</td> 
      <td>Runtime evaluation engine and logic interpreter for live node graph execution.</td> 
    </tr>
    <tr> 
      <td><b>Data Broker</b></td>      
      <td align="center">$\textsf{\color{#E62E2E}{v0.0}}$</td> 
      <td>System managing internal/external shared memory and cross-process data exchange.</td> 
    </tr>
  </tbody>
</table>

---

<br/>

## 📁 Project Structure

```text
├── .github/
│   ├── assets/
├── assets/
│   ├── tabler/
├── src/
│   └── editor/
│       ├── ANodeEnvironment/
│       ├── AWindow/
│       ├── Utility/
│       ├── CMakeLists.txt
│       └── main.cpp
├── .gitattributes
├── .gitignore
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE
└── README.md
```

See the full [Project Structure Documentation](docs/PROJECT_STRUCTURE.md) for a complete breakdown of all files and subsystems.

---

<br/>

## 📦 Architecture

Axolotool is built around a decoupled, thread-safe execution pipeline designed to separate high-frequency background data processing from Qt's main GUI render loop.

* **Low-Latency Streaming:** Asynchronous producer-consumer pipeline leveraging **C++23 coroutines** (`std::generator`) and static bounded queues for rate limiting.
* **Transactional Storage:** Embedded SQLite (WAL mode, 4NF schema) with thread-safe connection pooling and RAII transaction helpers.
* **Decoupled View:** Custom Qt GraphicsView layer isolating visual node rendering from database.

📖 For detailed subsystem breakdowns, dynamic loading mechanisms, and threading models, see the full **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** documentation.

---

<br/>

## ⚡ Features

### 1. Node Environment

<br/>

#### Nodes

<table>
  <thead>
    <tr> 
      <th width="170">Feature</th> 
      <th width="100">Status</th> 
      <th>Description</th> </tr>
  </thead>
  <tbody>
    <tr> 
      <td>Node Creation</td>    
      <td align="center">🟢 Done</td> 
      <td>Spawn a new node in the center of the workspace environment.</td> </tr>
    <tr> 
      <td>Node Deletion</td>     
      <td align="center">🟢 Done</td> 
      <td>Delete all selected nodes by pressing the <kbd>Delete</kbd> key.</td> </tr>
    <tr> 
      <td>Node Moving</td>       
      <td align="center">🟢 Done</td> 
      <td>Drag any selected node to move the entire active selection.</td> </tr>
    <tr> 
      <td>Selecting Nodes</td>   
      <td align="center">🟢 Done</td> 
      <td>Box-select nodes by dragging with <kbd>RMB</kbd>, or add individual nodes with <kbd>Ctrl</kbd> + <kbd>LMB</kbd>.</td> </tr>
    <tr> 
      <td>Unselecting Nodes</td> 
      <td align="center">🟢 Done</td>
      <td>Click empty workspace canvas to clear the current selection.</td>
    </tr>
  </tbody>
</table>

<br/>

#### Wires

<table>
  <thead>
    <tr>
      <tr>
        <th width="170">Feature</th> 
        <th width="100">Status</th>
        <th>Description</th>
      </tr>
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td>Wire Linking</td>  
      <td align="center">🟢 Done</td> 
      <td>Drag a temporary wire from an origin pin to a target pin. If valid, it snaps to the pin and creates a permanent connection upon release.</td>
    </tr>
    <tr> 
      <td>Wire Unlinking</td> 
      <td align="center">🟢 Done</td> 
      <td>Remove all wires connected to a pin by double-clicking (<kbd>LMB</kbd>).</td> 
    </tr>
  </tbody>
</table>

<br/>

#### Navigation

<table>
  <thead>
    <tr>
      <tr> 
        <th width="170">Feature</th> 
        <th width="100">Status</th> 
        <th>Description</th>
      </tr>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Canvas Panning</td> 
      <td align="center">🟢 Done</td> 
      <td>Pan around the workspace by dragging empty canvas with <kbd>LMB</kbd>.</td>
    </tr>
    <tr>
      <td>Auto-Scroll</td>    
      <td align="center">🟢 Done</td>
      <td>Click <kbd>MMB</kbd> and move the mouse to pan dynamically based on directional offset.</td> 
    </tr>
    <tr> 
      <td>Zooming</td>        
      <td align="center">🟢 Done</td> 
      <td>Zoom in and out using <kbd>Ctrl</kbd> + <kbd>Mouse Wheel</kbd>.</td> 
    </tr>
  </tbody>
</table>

<br/>

#### Streaming Data & Serialization

<table>
  <thead>
    <tr>
      <tr> 
        <th width="170">Feature</th>
        <th width="100">Status</th>
        <th>Description</th>
      </tr>
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td>Saving</td> 
      <td align="center">🟢 Done</td> 
      <td>Serialize active node positions, properties, and wire topology to the database.</td>
    </tr>
    <tr> 
      <td>Loading</td> 
      <td align="center">🟢 Done</td> 
      <td>Deserialize and reconstruct full node graph state asynchronously from persistence storage.</td> 
    </tr>
  </tbody>
</table>

<br/>

#### Database

<table>
  <thead>
    <tr>
      <tr> 
        <th width="170">Feature</th> 
        <th width="100">Status</th>
        <th>Description</th>
      </tr>
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td>Persistence</td>    
      <td align="center">🟢 Done</td> 
      <td>CRUD operations separating node graph Entity Data (Intrinsic/Shared) and Instance Data (Extrinsic/State).</td> 
    </tr>
    <tr> <td>Transactions</td>   
      <td align="center">🟢 Done</td> 
      <td>RAII transaction manager ensuring atomic graph operations and rollback safety.</td> 
    </tr>
    <tr>
      <td>Connection Pool</td> 
      <td align="center">🟢 Done</td> 
      <td>Thread-safe, RAII-leased SQLite connection pool supporting dynamic scaling and tuned WAL mode pragmas.</td>
    </tr>
  </tbody>
</table>

<br/>

#### Registry

<table>
  <thead>
    <tr>
      <tr> 
        <th width="170">Feature</th> 
        <th width="100">Status</th>
        <th>Description</th>
      </tr>
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td>Entity Cache</td>      
      <td align="center">🟢 Done</td> 
      <td>In-memory caching layer to eliminate redundant database queries during graph evaluation.</td> 
    </tr>
    <tr> 
      <td>Function Registry</td>
      <td align="center">🟢 Done</td> 
      <td>Centralized lookup and dispatcher for mapping executable node callbacks and logic functions.</td>
    </tr>
    <tr> 
      <td>Visual Registry</td>  
      <td align="center">🟢 Done</td> 
      <td>UI registry managing visible canvas elements, hidden items, and custom node visual templates.</td>
    </tr>
  </tbody>
</table>

---

<br/>

## 🤝 Contributing

![](https://img.shields.io/badge/contributions-paused-red?logo=github)

Thank you for your interest in Axolotool!

Axolotool is currently in early pre-1.0 development with core systems undergoing heavy iteration.  
**Code contributions (Pull Requests) are temporarily paused** until the core architecture stabilizes.

Check back once v1.0 releases for community contribution guidelines!

---

<br/>

## 🎯 What's Next?

### Plugin Manager

<table>
  <thead>
    <tr> 
      <th width="170">Feature</th> 
      <th width="140" align="center">Status</th> 
      <th>Description</th> 
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td><b>Architecture</b></td> 
      <td align="center">🟠 In-Progress</td> 
      <td>Design overall architecture and interface contracts for the Plugin Manager.</td> 
    </tr>
    <tr> 
      <td><b>Subsystem Drafts</b></td> 
      <td align="center">🟠 In-Progress</td> 
      <td>Draft core dynamic loading and lifecycle management components.</td> 
    </tr>
  </tbody>
</table>

<br/>

### Execution Graph

<table>
  <thead>
    <tr> 
      <th width="170">Feature</th> 
      <th width="140" align="center">Status</th> 
      <th>Description</th> 
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td><b>Architecture</b></td> 
      <td align="center">⚪ Planned</td> 
      <td>Design graph evaluation topology and data flow architecture.</td> 
    </tr>
    <tr> 
      <td><b>Runtime Engine</b></td> 
      <td align="center">⚪ Planned</td> 
      <td>Implement execution loop and node evaluation scheduler.</td> 
    </tr>
  </tbody>
</table>

<br/>

### Data Broker

<table>
  <thead>
    <tr> 
      <th width="170">Feature</th> 
      <th width="140" align="center">Status</th> 
      <th>Description</th> 
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td><b>Shared Memory</b></td> 
      <td align="center">⚪ Planned</td> 
      <td>Implement internal and external shared memory buffers.</td> 
    </tr>
  </tbody>
</table>

---

<br/>

## 🤖 AI-Assisted Development

> [!NOTE]
> This section describes author’s use of AI in this project.

AI tools are used as part of author’s development workflow, primarily as a learning resource (including but not limited to helping with searching in Qt documentation), and code review tool.

### Usage

AI is primarily used for:
* Learning Qt
* Code review
* Technical discussion
* Proofreading
* Occasional code generation

Author uses AI conversationally (via chat interface) rather than as an autonomous coding agent or IDE&#8209;integrated assistant. Author does not use GitHub Copilot or similar tools to generate code.

### Architecture

Architectural decisions are entirely made by author based on project’s requirements and constraints. AI was not used to design the system architecture, component structure, database schema, or overall organization. AI may occasionally be used to discuss or review individual implementation details, but it is never used as the source of the architecture.

### Generated Code

The vast majority of the implementation was written manually.  

There have been two occasions where AI generated an entire source file [\[1\]](src/editor/ANodeEnvironment/Storage/NDPool.h) [\[2\]](src/editor/ANodeEnvironment/Streaming/details/BoundedQueue.h), which are disclosed here for full transparency. Smaller AI&#8209;generated snippets and suggestions have also occasionally been incorporated into the project. Any AI&#8209;generated code that becomes part of the project is reviewed, understood, adapted where necessary, and then tested by author. AI output is never treated as authoritative.  

> At the time of writing this section, the project contains approximately 13.2k lines of code across 146 files.

### Responsibility

AI assistance does not replace understanding of the codebase. The author remains fully responsible for the project’s architecture, implementation, testing, maintenance, quality, and compliance.

---

<br/>

## ⚖️ License & Compliance

### Project License

This project is under MPL-2.0 License - see [LICENSE](LICENSE) file for details.

### Qt LGPLv3 Compliance

Axolotool uses the **Qt 6 Framework** under the terms of the GNU Lesser General Public License (LGPL) version 3. To maintain compliance:

* **Dynamic Linking:** Axolotool dynamically links to the unmodified Qt shared libraries (`.dll`, `.dylib`, `.so`). Axolotool does not statically link Qt.
* **No Modifications:** Axolotool does not modify the Qt framework source code.
* **User Rights:** Users retain the right to replace the dynamically linked Qt libraries with their own modified versions, as granted by the LGPLv3 license.
* **Source Code:** Axolotool's source code is available in this repository under the [MPL-2.0 license](LICENSE).
* **Qt Source:** The original Qt source code can be obtained directly from [The Qt Company](https://www.qt.io/download).

*Note: Axolotool is an independent open-source project and is not affiliated with or endorsed by The Qt Company.*

<br/>

### Third-Party Libraries

Axolotool incorporates the following third-party library:

<table>
  <thead>
    <tr> 
      <th width="150">Library</th> 
      <th width="200">License</th> 
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr> 
      <td align="center"><b><a href="https://github.com/gershnik/modern-uuid">modern-uuid</a></b></td> 
      <td align="center"><a href="https://github.com/gershnik/modern-uuid/blob/master/LICENSE">BSD 3-Clause License</a></td> 
      <td>C++20 header-only library for UUID generation and manipulation.</td> 
    </tr>
  </tbody>
</table>

---

<br/>

## 👩‍💻 Author

Olivia Heartfelt

<p align="left">
  <a href="https://github.com/OliviaHeartfelt"><img src="https://img.shields.io/badge/GitHub-@OliviaHeartfelt-181717?style=flat&logo=github" alt="GitHub"></a>
  &nbsp;
  <a href="https://www.linkedin.com/in/olivia-heartfelt"><img src="https://img.shields.io/badge/LinkedIn-@OliviaHeartfelt-181717?style=flat&logo=data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iNDQ4IiBoZWlnaHQ9IjQ0OCIgdmlld0JveD0iMCAwIDQ0OCA0NDgiIGZpbGw9Im5vbmUiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+CjxnIGNsaXAtcGF0aD0idXJsKCNjbGlwMF80OV81MSkiPgo8cGF0aCBkPSJNNjQgMEMyOC43IDAgMCAyOC43IDAgNjRWMzg0QzAgNDE5LjMgMjguNyA0NDggNjQgNDQ4SDM4NEM0MTkuMyA0NDggNDQ4IDQxOS4zIDQ0OCAzODRWNjRDNDQ4IDI4LjcgNDE5LjMgMCAzODQgMEg2NFpNNjkgMTcwLjJIMTM1LjVWMzg0SDY5VjE3MC4yWk0xNDAuNyAxMDIuNUMxNDAuNyAxMTIuNzExIDEzNi42NDQgMTIyLjUwMyAxMjkuNDI0IDEyOS43MjRDMTIyLjIwMyAxMzYuOTQ0IDExMi40MTEgMTQxIDEwMi4yIDE0MUM5MS45ODkyIDE0MSA4Mi4xOTY1IDEzNi45NDQgNzQuOTc2NCAxMjkuNzI0QzY3Ljc1NjIgMTIyLjUwMyA2My43IDExMi43MTEgNjMuNyAxMDIuNUM2My43IDkyLjI4OTIgNjcuNzU2MiA4Mi40OTY1IDc0Ljk3NjQgNzUuMjc2NEM4Mi4xOTY1IDY4LjA1NjIgOTEuOTg5MiA2NCAxMDIuMiA2NEMxMTIuNDExIDY0IDEyMi4yMDMgNjguMDU2MiAxMjkuNDI0IDc1LjI3NjRDMTM2LjY0NCA4Mi40OTY1IDE0MC43IDkyLjI4OTIgMTQwLjcgMTAyLjVaTTMxNy45IDM4NFYyODBDMzE3LjkgMjU1LjIgMzE3LjQgMjIzLjMgMjgzLjQgMjIzLjNDMjQ4LjggMjIzLjMgMjQzLjUgMjUwLjMgMjQzLjUgMjc4LjJWMzg0SDE3Ny4xVjE3MC4ySDI0MC44VjE5OS40SDI0MS43QzI1MC42IDE4Mi42IDI3Mi4zIDE2NC45IDMwNC42IDE2NC45QzM3MS44IDE2NC45IDM4NC4zIDIwOS4yIDM4NC4zIDI2Ni44VjM4NEgzMTcuOVoiIGZpbGw9IiMwQTY2QzIiLz4KPHBhdGggZD0iTTY5IDE3MC4ySDEzNS41VjM4NEg2OVYxNzAuMlpNMTQwLjcgMTAyLjVDMTQwLjcgMTEyLjcxMSAxMzYuNjQ0IDEyMi41MDMgMTI5LjQyNCAxMjkuNzI0QzEyMi4yMDMgMTM2Ljk0NCAxMTIuNDExIDE0MSAxMDIuMiAxNDFDOTEuOTg5MSAxNDEgODIuMTk2NSAxMzYuOTQ0IDc0Ljk3NjMgMTI5LjcyNEM2Ny43NTYyIDEyMi41MDMgNjMuNyAxMTIuNzExIDYzLjcgMTAyLjVDNjMuNyA5Mi4yODkyIDY3Ljc1NjIgODIuNDk2NSA3NC45NzYzIDc1LjI3NjRDODIuMTk2NSA2OC4wNTYyIDkxLjk4OTEgNjQgMTAyLjIgNjRDMTEyLjQxMSA2NCAxMjIuMjAzIDY4LjA1NjIgMTI5LjQyNCA3NS4yNzY0QzEzNi42NDQgODIuNDk2NSAxNDAuNyA5Mi4yODkyIDE0MC43IDEwMi41Wk0zMTcuOSAzODRWMjgwQzMxNy45IDI1NS4yIDMxNy40IDIyMy4zIDI4My40IDIyMy4zQzI0OC44IDIyMy4zIDI0My41IDI1MC4zIDI0My41IDI3OC4yVjM4NEgxNzcuMVYxNzAuMkgyNDAuOFYxOTkuNEgyNDEuN0MyNTAuNiAxODIuNiAyNzIuMyAxNjQuOSAzMDQuNiAxNjQuOUMzNzEuOCAxNjQuOSAzODQuMyAyMDkuMiAzODQuMyAyNjYuOFYzODRIMzE3LjlaIiBmaWxsPSJ3aGl0ZSIvPgo8L2c+CjxkZWZzPgo8Y2xpcFBhdGggaWQ9ImNsaXAwXzQ5XzUxIj4KPHJlY3Qgd2lkdGg9IjQ0OCIgaGVpZ2h0PSI0NDgiIGZpbGw9IndoaXRlIi8+CjwvY2xpcFBhdGg+CjwvZGVmcz4KPC9zdmc+Cg==" alt="LinkedIn"/></a>
</p>








