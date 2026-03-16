# Memory Profiling Commands

> `{BasePath}` = `/home/arijitbiswas/Desktop/CPP`

> **Note:** ASan conflicts with Valgrind/Heaptrack. Always rebuild without sanitizers before using these tools.

---

## Rebuild Without Sanitizers

```bash
cd {BasePath}/cmake-build-debug
cmake -DCMAKE_CXX_FLAGS="-g -fno-omit-frame-pointer" -DCMAKE_EXE_LINKER_FLAGS="" ..
make
```

---

## Valgrind: Leak & Memory Error Detection

```bash
# Basic leak check
valgrind {BasePath}/cmake-build-debug/CPP

# Full leak report with origins
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose {BasePath}/cmake-build-debug/CPP

# Save report to a file
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_report.txt {BasePath}/cmake-build-debug/CPP
```

## Valgrind Massif: Heap Usage Over Time

```bash
# Step 1: Profile
valgrind --tool=massif --pages-as-heap=yes {BasePath}/cmake-build-debug/CPP

# Step 2: View as text (replace XXXXX with actual number)
ms_print massif.out.XXXXX

# Step 3: View in GUI (install first: sudo apt install massif-visualizer)
massif-visualizer massif.out.XXXXX
```

---

## Heaptrack: Heap Profiler

```bash
# Install (if not installed)
sudo apt install heaptrack heaptrack-gui

# Step 1: Profile (use full path)
heaptrack {BasePath}/cmake-build-debug/CPP

# Step 2: Open GUI (replace XXXXX with actual number)
heaptrack_gui heaptrack.CPP.XXXXX.gz
```

---

## Restore Sanitizers (for normal development)

Add these two lines to `CMakeLists.txt`:

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined -g -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,leak,undefined")
```
