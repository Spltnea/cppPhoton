# The Photon Programming Language

```photon
fn _start() {
    println("Hello ! Photon :) ");

    loop {
        asm { hlt }
    }
}
```

## Project Presentation
The Photon Programming Language is a bare-metal targeted language designed to eliminate the common pitfalls and quirks of bare-metal development in C and C++.

The reference compiler is implemented in **modern C++23**, completely utilizing **C++20/C++23 Modules (`.cppm`)** and standard library module imports (`import std;`).

---

## Features

The Photon Compiler provides:

- **Concise, expressive syntax:**
```cpp
// Immutable pointer to a mutable address in C++
int *const bar = &c;
```

```photon
// The same concept in Photon
const bar : i8* -> &!c;
```

*(A complete overview will be available in the upcoming manual.)*

- **Clear, actionable error diagnostics:**
```text
(line : col) var x : i8* = &! y;
                           ^^

[ERROR] : Mutable reference '&!' to constant 'y' :
          const y : i8 = 15;
Help :
=> Change the mutable reference to a constant reference:
    var x : i8* = & y;
                  ^

=> Change 'y' to a variable:
    var y : i8 = 15;
```

- **Compile-time safety checks** for critical code paths.
- **Fast, modern compiler architecture** built without legacy textual header overhead.

---

## Safety
The compiler implements built-in compile-time verification to detect and prevent hardware-level and runtime faults before execution, including:
1. Triple faults
2. Segmentation faults
3. Infinite hardware reset loops
4. Common critical memory errors (e.g., buffer overflows)

---

## Requirements

Building `cppPhoton` requires bleeding-edge C++23 standard module support:

* **Compiler:** **Clang 19+** with **`libc++`** (Clang's `libc++` is mandatory for `import std;`)
* **Build System:** **CMake 3.30+** (Required for experimental `import std;` support)
* **Generator:** **Ninja 1.11+** (Traditional Makefiles do not support module scanning)
* **Language Server (Optional):** **`clangd-19`**

---

## Building

### 1 : Install the Toolchain

#### Ubuntu / Debian / Linux Mint
```bash
# 1. Install Clang 19, libc++, Ninja, and Git
sudo apt update
sudo apt install -y clang-19 clangd-19 libc++-19-dev libc++abi-19-dev ninja-build git

# 2. Ensure CMake >= 3.30 is installed
# (If your system apt has an older version, install via pip or the official Kitware repo):
cmake --version
# pip install --upgrade cmake
```

#### Arch Linux / Manjaro
```bash
sudo pacman -Syu --needed clang libc++ libc++abi cmake ninja git
```

#### Fedora / RHEL
```bash
sudo dnf install -y clang libcxx-devel libcxxabi-devel cmake ninja-build git
```

#### macOS (Homebrew)
```bash
brew install llvm cmake ninja git

# Expose LLVM Clang to your PATH
export PATH="$(brew --prefix llvm)/bin:$PATH"
```

---

### 2 : Clone the Repository

```bash
git clone https://github.com/Spltnea/cppPhoton.git
cd cppPhoton
```

---

### 3 : Configure & Build (Using Presets)

The repository uses **CMake Presets (v6)** to automatically wire up `clang++-19`, `Ninja`, and `-stdlib=libc++`.

#### Release Mode:
```bash
# Configure
cmake --preset release

# Build
cmake --build --preset release
```

#### Debug Mode *(Enables debug definitions)*:
```bash
# Configure
cmake --preset debug

# Build
cmake --build --preset debug
```

<details>
<summary>Click to view manual configuration without presets</summary>

```bash
cmake -B build/release -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=clang++-19 \
  -DCMAKE_C_COMPILER=clang-19 \
  -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
  -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build build/release
```
</details>

---

### 4 : Running

Execute the compiler binary directly:

```bash
# Run Release build
./build/release/cppPhoton

# Run Debug build
./build/debug/cppPhoton
```

---

## IDE & Editor Setup (VS Code)

For the best developer experience with C++23 modules:

1. Install the official **clangd** extension (`llvm-vs-code-extensions.vscode-clangd`).
2. Disable the default Microsoft C/C++ IntelliSense engine.
3. Ensure `.vscode/settings.json` points to `clangd-19` with module support enabled:

```json
{
  "clangd.path": "/usr/bin/clangd-19",
  "clangd.arguments": [
    "--compile-commands-dir=build/release",
    "--experimental-modules-support",
    "--background-index"
  ]
}
```

---

## Resources, Documentation & AI Involvement

### References
- [cppreference.com](https://cppreference.com/)
- [LLVM / Clang Modules Documentation](https://clang.llvm.org/docs/StandardCPlusPlusModules.html)
- [CMake C++ Modules Guide](https://cmake.org/cmake/help/latest/manual/cmake-cxxmodules.7.html)
- [Stack Overflow](https://stackoverflow.com/)
- [r/cpp](https://www.reddit.com/r/cpp/)

### Note on AI
AI was utilized strictly as an auxiliary tool for:
- Writing and structuring modern C++ module build configuration files (`CMakeLists.txt` and presets).
- Providing isolated, synthetic code snippets for study.
- Cross-referencing language design patterns across existing compilers.
- Serving as a technical critique partner.

95% of the codebase and documentation is written by hand. Coding is an art form—it is not merely about generating working syntax, but expressing design intent clearly.

*Any file, function, or snippet directly generated or modified by AI will be explicitly marked with an `// AI Generated | Modified` header comment.*