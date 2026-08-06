# CHIP-8 Emulator

This project is intentionally learning-first. We will build an interpreter for the CHIP-8 virtual machine a small, testable piece at a time rather than transcribing an existing implementation.

## Learning path

1. Model the CHIP-8 machine state and reset invariants.
2. Load a ROM safely into memory and inspect its first instructions.
3. Implement the fetch-decode-execute cycle with trace output.
4. Add opcode families in small groups, with focused tests.
5. Add the 60 Hz timers, input mapping, drawing, and sound.
6. Run public test ROMs, then document any compatibility choices.

Start with [`docs/01-foundations.md`](docs/01-foundations.md). Keep a short decision log for choices such as draw wrapping and shift behavior: different CHIP-8 interpreters historically differed.

## Boundaries for the first version

- Target classic CHIP-8 only; do not add Super-CHIP or XO-CHIP features yet.
- Keep emulation core independent of the windowing/audio library.
- Treat ROMs as external test assets; only commit ROMs you have permission to distribute.

## Notes:
- CHIP-8 instructions, i.e. Opcodes are 2 bytes, but each memory slot is 1 byte, so first we need to combine two bytes:
- To combine two hex bytes into a single 16-bit value, you must shift the Most Significant Byte (MSB) to the left by 8 bits and then combine it with the Least Significant Byte (LSB) using a bitwise OR or addition operation
- in formula: Result = (MSB << 8)|LSB

- read ROM files:
- Throwing an exception signals that a fatal error occurred which the current function does not know how to fix. It halts execution and bubbles up the call stack until a catch block handles it. If no one catches it, the program crashes immediately.


