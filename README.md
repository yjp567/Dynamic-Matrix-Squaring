# Dynamic Matrix Squaring

This project implements dynamic parallel matrix squaring using multiple synchronization methods (TAS, CAS, Bounded CAS, Atomic) in C++. It was developed as part of the **Operating Systems–II** course assignment.

---

## Compiling and Executing the Code

1. Open the terminal in the directory where `Assgn3_Src-CS22BTECH11047.cpp` and `inp.txt` are located.
2. To compile the main source file:
   ```
   g++ Assgn3_Src-CS22BTECH11047.cpp
   ```
3. To execute the program:
   ```
   ./a.out
   ```
4. The output will be written to a file named `out.txt` in the same directory (it will be overwritten if it already exists).

---

## Generating the Input File (`inp.txt`)

1. A helper file `helper.cpp` is provided to generate the input matrix.
2. You can modify values of `n`, `k`, and `rowInc` in `helper.cpp` as per your requirements.
3. Now, compile the helper program using:
   ```
   g++ helper.cpp
   ```
4. Then execute it using:
   ```
   ./a.out
   ```
5. This will generate an input file named `inp.txt` with your specified values of `n`, `k`, and `rowInc`.

6. Note: The matrix values are randomly chosen integers between **0 and 9** (inclusive).

---

## Files

- `Assgn3_Src-CS22BTECH11047.cpp` – Main implementation
- `helper.cpp` – Input generator
- `inp.txt` – Input matrix file (auto-generated)
- `out.txt` – Output matrix file (auto-generated)

---
