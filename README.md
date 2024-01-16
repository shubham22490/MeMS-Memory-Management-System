# MeMS: Memory Management System

# MeMS - Memory Management System

## Overview
MeMS (Memory Management System) is a C implementation of a custom memory management system that allows dynamic memory allocation and deallocation. The system uses a combination of linked lists and memory mapping to efficiently manage memory blocks.

## Functions

### `mems_init`
Initializes the MeMS system by setting initial parameters such as the head of the free list, starting MeMS virtual address, and global variables.

### `mems_finish`
Cleans up the MeMS system by unmapping the allocated memory using the `munmap` system call.

### `createSub`
Creates a sub-block with a specified size and type (PROCESS or HOLE).

### `createBlock`
Creates a memory block with a specified size and allocates sub-blocks for process and hole using `createSub`.

### `checkHole`
Checks for a hole within a memory block that can fulfill the required size.

### `mems_malloc`
Allocates memory of the specified size by reusing segments from the free list or using `mmap` to allocate more memory on the heap.

### `mems_print_stats`
Prints statistics about the MeMS system, including pages used, unused space, main chain details, and sub-chain lengths.

### `mems_get`
Returns the MeMS physical address mapped to the given MeMS virtual address.

### `mems_free`
Frees up the memory pointed by the virtual address and adds it to the free list.

## Usage
To use the MeMS system, include the provided header file in your C program and call the necessary functions. Make sure to follow the provided documentation for each function to understand their functionality and parameters. Given one example.c file to understand the same.

### How to run the exmaple.c
After implementing functions in mems.h follow the below steps to run example.c file
```
$ make
$ ./example
```
---
### Format of mems_mems_print_stats
* For every Subchain in the free list print the data as follows 
```
MAIN[starting_mems_vitual_address:ending_mems_vitual_address] -> <HOLE or PROCESS>[starting_mems_vitual_address:ending_mems_vitual_address] <-> ..... <-> NULL
```
* After printing the whole freelist print the following stats
```
Page used: <Total Pages used by the user>
Space unused: <Total space mapped but not used by user>
Main Chain Length: <Length of the main chain>
Sub-chain Length array: <Array denoting the length of the subchains>
```
* Analyse the output of the example.c file below to get a better understanding of the output
---
### Example Output
PAGE_SIZE= 4096 Bytes

Starting MeMS Virtual Address= 1000

![Example Output](example_output.jpg)
