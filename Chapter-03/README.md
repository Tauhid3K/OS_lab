# Chapter 3: Processes — Code Examples, Exercises & Projects

This repository contains full, working implementations of all code examples, practice exercise simulations, end-of-chapter exercises, programming problems, and programming projects from **Chapter 3 ("Processes")** of *Operating System Concepts* (10th Edition) by Silberschatz, Galvin, and Gagne.

---

## Directory Overview

```
Chapter-03/
├── 01_memory_layout.c             # Section 3.1.1: Memory layout box
├── 02_linux_process_creation.c    # Figure 3.8: UNIX fork() + execlp("/bin/ls") + wait()
├── 03_windows_process_creation.c  # Figure 3.10: Windows CreateProcess()
├── 04_producer_consumer_buffer.c  # Section 3.5.1: Bounded-buffer declarations & producer
├── 05_consumer_shared_memory.c    # Figure 3.13: Consumer using shared memory
├── 06_message_passing_basic.c     # Section 3.5.2: Basic message passing model
├── 07_direct_communication.c      # Section 3.5.2: Direct symmetric IPC
├── 08_asymmetric_communication.c  # Section 3.5.2: Asymmetric IPC
├── 09_indirect_communication.c    # Section 3.5.2: Indirect communication (mailboxes)
├── 10_mailbox_operations.c        # Section 3.5.2: Mailbox lifecycle operations
├── 11_producer_message_passing.c  # Figure 3.14: Producer using message passing
├── 12_posix_shm_producer.c        # Figure 3.16: POSIX shared-memory producer
├── 13_posix_shm_consumer.c        # Figure 3.17: POSIX shared-memory consumer
├── 14_unix_pipe.c                 # Figure 3.21 & 3.22: UNIX ordinary pipe (parent/child)
├── 15_windows_pipe_parent.c       # Figure 3.23 & 3.24: Windows anonymous pipe (parent side)
├── 16_windows_pipe_child.c        # Figure 3.25: Windows anonymous pipe (child side)
├── 17_mach_message_passing.c      # Section 3.6.1 & Fig 3.18: Mach IPC message structure
│
├── java_sockets/
│   ├── DateServer.java            # Figure 3.27: Java TCP DateServer (port 6013)
│   └── DateClient.java            # Figure 3.28: Java TCP DateClient
│
├── exercise_3_01_fork_value.c     # Practice Exercise 3.1 (Fig 3.30): LINE A output
├── exercise_3_02_triple_fork.c    # Practice Exercise 3.2 (Fig 3.31): Three sequential forks
├── exercise_3_11_loop_fork.c      # Exercise 3.11 (Fig 3.32): 4-iteration fork loop
├── exercise_3_12_exec_line_j.c    # Exercise 3.12 (Fig 3.33): execlp and LINE J reachability
├── exercise_3_13_pid_tracking.c   # Exercise 3.13 (Fig 3.34): PID tracking at lines A–D
├── exercise_3_16_array_fork.c     # Exercise 3.16 (Fig 3.35): Modifying array after fork
│
├── problem_3_18_zombie.c          # Problem 3.18: Zombie process creation
├── problem_3_19_time_shm.c        # Problem 3.19: Time command via POSIX shared memory
├── problem_3_19_time_pipe.c       # Problem 3.19: Time command via ordinary pipe
├── problem_3_20_pid_manager.c     # Problem 3.20: PID Manager (allocate_map, allocate_pid, release_pid)
├── problem_3_21_collatz_fork.c    # Problem 3.21: Collatz sequence generated in child process
├── problem_3_22_collatz_shm.c     # Problem 3.22: Collatz sequence using POSIX shared memory
├── problem_3_23_qotd_server.c     # Problem 3.23: Quote-of-the-Day server (port 6017)
├── problem_3_24_haiku_server.c    # Problem 3.24: Haiku TCP server (port 5575)
├── problem_3_25_echo_server.c     # Problem 3.25: TCP Echo server (port 6018)
├── problem_3_26_pipe_reverse_case.c # Problem 3.26: Two-way ordinary pipe IPC (case inversion)
├── problem_3_27_filecopy.c        # Problem 3.27: File copy utility using ordinary pipe
│
├── project_1_shell.c              # Programming Project 1: Complete UNIX Shell (osh)
│
└── kernel_projects/               # Linux Kernel Modules (Projects 2, 3 & Kernel Data Structures)
    ├── kernel_pid_info.c          # Project 2 (Fig 3.37): /proc/pid task info module
    ├── kernel_list_tasks.c        # Project 3: Linear & DFS task list traversal
    ├── kernel_color_list.c        # Kernel Data Structures: struct color linked list
    ├── kernel_collatz.c           # Kernel Data Structures: Collatz with module_param
    └── Makefile                   # Kernel module build script (LLVM/Clang or GCC)
```

---

## How to Build

### 1. User-Space Programs & Java Sockets
Run `make` inside `Chapter-03`:
```bash
make
```
This builds all C binaries and compiles Java socket classes.

### 2. Kernel Modules
Run `make` inside `Chapter-03/kernel_projects`:
```bash
cd kernel_projects
make
```
This generates the `.ko` kernel objects:
- `kernel_pid_info.ko`
- `kernel_list_tasks.ko`
- `kernel_color_list.ko`
- `kernel_collatz.ko`

---

## Running Key Programs

- **UNIX Shell Project:**
  ```bash
  ./project_1_shell
  osh> ls -la
  osh> ls -l | grep c
  osh> ls > files.txt
  osh> !!
  osh> exit
  ```

- **Collatz Sequences:**
  ```bash
  ./problem_3_21_collatz_fork 35
  ./problem_3_22_collatz_shm 35
  ```

- **Time Utility (SHM & Pipe):**
  ```bash
  ./problem_3_19_time_pipe sleep 1
  ./problem_3_19_time_shm ls -la
  ```

- **Two-Way Pipe Case Inverter:**
  ```bash
  ./problem_3_26_pipe_reverse_case "Operating Systems 2026!"
  ```

- **File Copy with Pipe:**
  ```bash
  ./problem_3_27_filecopy 14_unix_pipe.c copy.c
  ```

- **Java Date Server & Client:**
  ```bash
  java -cp java_sockets DateServer &
  java -cp java_sockets DateClient
  ```
