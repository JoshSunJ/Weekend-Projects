# CHIP-8 Foundations

Read this before opening a graphics library or implementing an opcode.

## 1. What you are building

CHIP-8 is a small virtual machine specification, originally implemented as an interpreter on several 1970s systems. A CHIP-8 ROM is not code your computer executes directly. Your program is the *host*; it reads the ROM's two-byte CHIP-8 instructions and updates an in-memory model of the *guest* machine.

That distinction is the core idea behind emulation:

```
ROM bytes -> fetch instruction -> decode fields -> change CHIP-8 state -> present I/O
```

The important result is deterministic state transitions. Given identical starting state, input events, random-number sequence, and elapsed timer ticks, the same ROM should evolve identically.

## 2. Numbers and bit operations

You need to be comfortable reading hexadecimal. One hex digit represents four bits; two hex digits make an 8-bit byte. For example, `0xAB` is one byte and `0x2ABC` is a 16-bit value.

An instruction is 16 bits, read from two adjacent memory bytes in big-endian order: the byte at `PC` is the high byte and the byte at `PC + 1` is the low byte. Combining them means shifting the high byte left eight positions and joining it with the low byte.

Every opcode can be described with reusable fields:

| Field | Meaning | Example extraction from `0xABCD` |
| --- | --- | --- |
| `X` | register number | `B` |
| `Y` | register number | `C` |
| `N` | low nibble (4 bits) | `D` |
| `KK` | low byte (8 bits) | `CD` |
| `NNN` | low 12 bits (address) | `BCD` |

Practice deriving those fields by hand from `0x6A3F`: the opcode family is `6`, `X` is `A`, and the immediate byte is `0x3F`.

The operations that matter most are:

- **Masking** (`value AND mask`) isolates bits.
- **Shifting** moves bits left or right; capture any bit you need *before* shifting it away.
- **XOR** toggles a bit: equal bits become 0, different bits become 1. CHIP-8 sprite drawing relies on it.
- **Overflow is deliberate.** An 8-bit register holds `0x00`–`0xFF`; arithmetic results may need truncating to eight bits while a flag records carry or borrow.

## 3. The machine state

Before coding, draw this on paper and explain what each part represents:

- 4 KiB memory, addressed from `0x000` to `0xFFF`.
- 16 general-purpose 8-bit registers `V0` through `VF`. `VF` is also a flag register for several instructions, so never assume it keeps an unrelated value.
- `I`, a 16-bit index/address register.
- `PC`, the 16-bit program counter: address of the *next* opcode.
- A 16-entry call stack and stack pointer.
- Delay and sound timers, each ticking down at exactly 60 Hz when nonzero.
- A 16-key pressed/not-pressed keypad state.
- A monochrome 64 × 32 display buffer.
- A random-byte source (inject it so tests can be repeatable).

Establish reset invariants before loading a ROM: all state cleared, `PC = 0x200`, font data copied to an agreed location (commonly `0x050`), and a cleared display. ROM instructions begin at `0x200`; the lower region was used by the original interpreter.

## 4. Execution is not timing

There are at least two independent clocks:

- **CPU stepping:** execute opcodes at a chosen emulation rate. Start with a configurable rate rather than assuming display frame rate equals CPU rate.
- **Timers:** decrement delay and sound timers at 60 Hz, regardless of CPU rate.

Separating those is essential. Tying timers to one executed instruction per frame makes ROM behavior depend on the host's speed. In a real-time main loop, measure elapsed time, accumulate it, execute as many CPU steps as the CPU accumulator permits (with a sensible cap), and apply every due 60 Hz timer tick.

## 5. Control flow and the stack

Because each instruction is two bytes, ordinary execution advances `PC` by two. Make one convention and use it consistently:

1. Fetch the opcode at the current `PC`.
2. Advance `PC` to the following instruction.
3. Execute; a jump/call/return/skip may overwrite or further advance `PC`.

For a call, push the already-advanced `PC`, then set `PC` to the target address. For a return, decrement the stack pointer first, then restore `PC` from the stack. Test nested calls and stack bounds early.

## 6. Display model

The `DXYN` instruction draws `N` bytes from memory beginning at `I`. Each byte is one horizontal row of eight sprite bits, most-significant bit first. For every 1 bit, XOR the corresponding display pixel.

Two consequences define the behavior:

- Drawing the same sprite twice erases it.
- If a lit pixel is toggled off, set `VF` to 1 to report a collision; otherwise set it to 0 before drawing.

Whether off-screen sprite pixels wrap or clip is a compatibility choice. Choose one documented profile at first, then use test ROMs to validate it.

## 7. Compatibility is part of emulation

There is no one behavior shared by every historical CHIP-8 interpreter. The usual early differences are:

- Whether shift instructions use `VX` or `VY` as their source.
- Whether `FX55` and `FX65` increment `I`.
- Whether drawing wraps around screen edges or clips.
- What `BNNN` adds to the base address.

Do not hide these choices in scattered opcode code. Put them in an explicit compatibility configuration and write the chosen behavior in a decision log. Start with one well-defined classic profile; add variants only when a test ROM demonstrates the need.

## 8. A learning-first implementation order

1. Build a state object and a reset operation. Unit test the reset invariants.
2. Implement a size-checked ROM loader. Reject ROMs larger than the available region.
3. Implement one `step` operation that fetches and decodes, initially with a trace line showing `PC`, opcode, and relevant fields.
4. Add simple load, add, and jump instructions; test state after exactly one step.
5. Add calls, returns, and skip instructions; test `PC` and stack contents.
6. Add arithmetic and flags; test boundary values such as `0xFF + 1`.
7. Add memory, font, random, and keypad instructions.
8. Add drawing, then timers and platform integration.

At every stage, use tiny hand-written byte sequences before running a full game. A full ROM is a great integration test but a poor first debugger.

## 9. Questions to answer before writing the first opcode

- What language and window/input library will you use, and why?
- How will the platform layer pass key events to the core without the core knowing about keyboard scan codes?
- What API makes a single CPU step observable in a test?
- Which machine state should a trace print after every step?
- Which compatibility profile will version one follow?
- How will you ensure timers are 60 Hz even on a slow or fast computer?

Write your answers in a decision log. They will make debugging much more illuminating than copying a finished emulator.
