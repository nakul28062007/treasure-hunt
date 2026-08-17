# Treasure Hunt — C Lab Exercises

A series of C programs built around a single evolving problem: a "Treasure Hunt"
using arrays of structures. Each version builds on the last, introducing new
C concepts (pointers, matrices) while keeping the core problem the same.

## Problem Statement

Store details of five treasures (name, X location, Y location) using an array
of structures. Given a starting location (x, y), compute and print the taxicab
distance from the starting point to each treasure, and identify the nearest one
via linear search.

- X, Y are integers in the range 0–9 (inclusive), validated on input.
- Taxicab distance: `|x1 - x2| + |y1 - y2|`

## Versions

| Folder       | Concept Focus                          | Description                                                                 |
|--------------|-----------------------------------------|-------------------------------------------------------------------------------|
| `v1-structs` | Arrays, Structures, Loops, If/Else      | Baseline implementation using nested structs (`Location` inside `Treasure`). Linear search for nearest treasure. |
| `v2-pointers`| Pointers                                | Same functionality, refactored to pass/manipulate data via pointers instead of array indexing. |
| `v3-matrix`  | 2D Arrays / Matrix                      | Extends the grid into a matrix representation to visualize or transform treasure positions. |

## Build & Run

Each version is a standalone `.c` file.

```bash
gcc v1-structs/treasure_hunt.c -o v1-structs/treasure_hunt
./v1-structs/treasure_hunt
```

(Repeat per version, adjusting the path.)

## Topics Covered

- Arrays of structures
- Nested structures (`struct Location` inside `struct Treasure`)
- Input validation with `do-while` loops
- Functions and pass-by-value vs pass-by-reference (pointers)
- Linear search (O(n))
- 2D arrays / matrix representation

## Notes

Built as part of a Data Structures & Algorithms lab. Each version lives in its
own folder/branch so the progression (structs → pointers → matrix) stays visible
in commit history.