#include "chip8.hpp"
#include <algorithm>
#include <fstream>   // std::ifstream
#include <vector>    // temporary byte buffer
#include <stdexcept> // std::runtime_error

Chip8::Chip8() {
    reset();
}

void Chip8::reset() {
    memory_.fill(0);
    registers_.fill(0);
    stack_.fill(0);
    keypad_.fill(0);
    display_.fill(0);

    index_ = 0;
    program_counter_ = program_start;
    stack_pointer_ = 0;
    delay_timer_ = 0;
    sound_timer_ = 0;
}

// public getter
std::uint16_t Chip8::program_counter() const {
    return program_counter_;
}

void Chip8::load_rom(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) {
        throw std::runtime_error("Could not open ROM: " + path.string());
    }

    std::streamsize rom_size = file.tellg();

    if (rom_size < 0) {
        throw std::runtime_error("Could not determine ROM size: " + path.string());
    }

    const auto rom_bytes = static_cast<std::size_t>(rom_size);
    const auto available_memory = memory_size - program_start;
    if (rom_bytes > available_memory) {
        throw std::runtime_error("ROM is too large for CHIP-8 memory"); // Throwing an exception signals that a fatal error occurred which the current function does not know how to fix. It halts execution and bubbles up the call stack until a catch block handles it. If no one catches it, the program crashes immediately.
    }

    std::vector<std::uint8_t> buffer(rom_bytes);

    file.seekg(0, std::ios::beg); // moves from the file’s end—where we measured its size—back to the beginning

    file.read(reinterpret_cast<char*>(buffer.data()), rom_size); // The read function takes two parameters: A pointer to the memory location where the data should be stored. The number of bytes to read
    if (!file) {
        throw std::runtime_error("Could not read ROM: " + path.string());
    }

    reset();
    std::copy(buffer.begin(), buffer.end(), memory_.begin() + program_start); // read succeed, puts those bytes into CHIP‑8 memory beginning at 0x200
}

std::uint8_t Chip8::memory_at(std::uint16_t address) const {
    if (address >= memory_size) {
        throw std::out_of_range("CHIP-8 memory address is out of range");
    }

    return memory_[address];
}

// std::uint16_t Chip8::fetch_opcode() const{
//     if(program_counter < memory_size - 1) {
//         std::uint8_t high_byte = memory_[program_counter];
//         std::uint8_t low_byte = memory_[program_counter + 1];
//         static_cast<uint16_t>(high_byte);
//         std::uint16_t opcode = (high_byte << 8) | low_byte;
//         return opcode;
//     }

//     return;
// };

std::uint16_t Chip8::fetch_opcode() const { // At this moment, your fetch_opcode() does not change PC. So if you call it ten times, it will fetch 0x00E0 ten times.
    if (program_counter_ >= memory_size - 1) {
        throw std::out_of_range("Not enough memory remaining to fetch an opcode");
    }

    const std::uint8_t high_byte = memory_[program_counter_];
    const std::uint8_t low_byte = memory_[program_counter_ + 1];

    const std::uint16_t opcode =
        (static_cast<std::uint16_t>(high_byte) << 8) | low_byte;

    return opcode;
}

void Chip8::execute_opcode(std::uint16_t opcode) {
    switch (opcode & 0xF000) {
    case 0x6000: { // 0x6XKK instruction load kk into x register
        const auto x = static_cast<std::uint8_t>((opcode & 0x0F00) >> 8);
        const auto kk = static_cast<std::uint8_t>(opcode & 0x00FF);

        registers_[x] = kk; //registers store uint8_t
        return;
    }
    default:
        throw std::runtime_error("Unsupported CHIP-8 opcode");
    }
}

std::uint8_t Chip8::register_at(std::uint8_t index) const {
    if (index >= register_count) { // bounds-checked
        throw std::out_of_range("CHIP-8 register index is out of range");
    }

    return registers_[index];
}

std::uint16_t Chip8::cycle() {
    const std::uint16_t opcode = fetch_opcode(); //Read the instruction at the current PC.

    program_counter_ += 2;

    execute_opcode(opcode);

    return opcode;
}
