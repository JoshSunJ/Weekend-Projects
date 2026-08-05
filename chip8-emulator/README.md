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
