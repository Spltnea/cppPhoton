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
- **Portable, multi-architecture compiler** powered by LLVM.

---

## Safety
The compiler implements built-in compile-time verification to detect and prevent hardware-level and runtime faults before execution, including:
1. Triple faults
2. Segmentation faults
3. Infinite hardware reset loops
4. Common critical memory errors (e.g., buffer overflows)

---

## Resources, Documentation & AI Involvement

### References
- [cppreference.com](https://cppreference.com/)
- [cplusplus.com](https://cplusplus.com/)
- [LLVM Documentation](https://llvm.org/docs/)
- [Stack Overflow](https://stackoverflow.com/)
- [r/cpp](https://www.reddit.com/r/cpp/)
- [GeeksforGeeks C++](https://www.geeksforgeeks.org/cpp/c-plus-plus/)

### Note on AI
AI was utilized strictly as an auxiliary tool for:
- Writing and structuring build configuration files (`CMakeLists.txt` and presets).
- Parsing and navigating dense LLVM APIs and documentation.
- Providing isolated, synthetic code snippets for study.
- Cross-referencing language design patterns across existing compilers.
- Serving as a technical critique partner.

95% of the codebase and documentation is written by hand or adapted from public open-source implementations. Coding is an art form—it is not merely about generating working syntax, but expressing design intent clearly.

*Any file, function, or snippet directly generated or modified by AI will be explicitly marked with an `// AI Generated | Modified` header comment.*

---

## Building

### 1 : Install the Toolchain

#### Ubuntu, Debian, Linux Mint
```bash
sudo apt update
sudo apt install -y build-essential clang llvm-dev libclang-dev libzstd-dev libffi-dev cmake ninja-build git
```

#### Fedora, RHEL, CentOS Stream, Rocky Linux
```bash
sudo dnf install -y gcc-c++ clang llvm-devel libzstd-devel libffi-devel cmake ninja-build make git
```

#### Arch Linux, Manjaro
```bash
sudo pacman -Syu --needed base-devel clang llvm llvm-libs zstd libffi cmake ninja git
```

#### macOS
```bash
# 1. Install Apple Command Line Tools
xcode-select --install

# 2. Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 3. Install dependencies
brew install llvm cmake ninja git zstd libffi

# 4. Expose Homebrew LLVM to your environment
export PATH="$(brew --prefix llvm)/bin:$PATH"
export LLVM_DIR="$(brew --prefix llvm)/lib/cmake/llvm"
```

#### Windows (Run PowerShell as Administrator)
```powershell
# 1. Install Visual Studio Build Tools (MSVC compiler & Windows SDK)
winget install --id Microsoft.VisualStudio.2022.BuildTools --override "--passive --config --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"

# 2. Install LLVM, CMake, Ninja, and Git
winget install --id LLVM.LLVM -e
winget install --id Kitware.CMake -e
winget install --id Ninja-build.ninja -e
winget install --id Git.Git -e
```
*Restart PowerShell after installation.*

---

### 2 : Clone the Repository

```bash
git clone https://github.com/Spltnea/cppPhoton.git
cd cppPhoton
```

---

### 3 : Configure the Project

> **macOS Reminder:** If using Homebrew LLVM, ensure `export LLVM_DIR="$(brew --prefix llvm)/lib/cmake/llvm"` is set before configuring.

#### Option A: Using CMake Presets (Recommended)

* **Release Mode:**
  ```bash
  cmake --preset release
  ```

* **Debug Mode** *(enables `#if DEBUG` and `#ifdef DEBUG` macros)*:
  ```bash
  cmake --preset debug
  ```

#### Option B: Manual Configuration

<details>
<summary>Click to view manual commands</summary>

* **Linux / macOS (Clang & Ninja):**
  ```bash
  cmake -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
  ```

* **Linux (GCC & Ninja):**
  ```bash
  cmake -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc
  ```

* **Windows (PowerShell / Developer Command Prompt):**
  ```powershell
  # Using MSVC + Ninja
  cmake -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release

  # Using Clang + Ninja
  cmake -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++
  ```

*(To configure for Debug, replace `Release` and `build/release` with `Debug` and `build/debug`)*
</details>

---

### 4 : Compile and Link

#### If using Presets:
```bash
# Build Release
cmake --build --preset release

# Build Debug
cmake --build --preset debug
```

#### If configured manually:
```bash
# Release
cmake --build build/release -j

# Debug
cmake --build build/debug -j
```

---

### 5 : Running

Execute the binary from the project root:

#### Linux / macOS
```bash
# Run Release build
./build/release/cppPhoton

# Run Debug build
./build/debug/cppPhoton

# In case of permission issues:
chmod +x ./build/release/cppPhoton
```

#### Windows (PowerShell)
```powershell
# Run Release build
.\build\release\cppPhoton.exe

# Run Debug build
.\build\debug\cppPhoton.exe

# In case of permission issues:
icacls ".\build\release\cppPhoton.exe" /grant:r "${env:USERNAME}:RX"
``` 