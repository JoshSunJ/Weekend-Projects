#include "chip8.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>

int main() {
    Chip8 chip8;

    std::ofstream rom_file("test.rom", std::ios::binary);
    if (!rom_file) {
        throw std::runtime_error("Can't open test ROM");
    }

    std::uint8_t chip8_program[] = {
        0x60, 0x05,
        0x61, 0x0A
    };

    // Write the CHIP-8 opcodes to the file
 

    rom_file.write(
        reinterpret_cast<const char*>(chip8Program), sizeof(chip8Program)
    );
    rom_file.close();

    chip8.load_rom("test.rom");

    const auto opcode = chip8.fetch_opcode();

    std::cout << "Fetched opcode: 0x" << std::hex << chip8.fetch_opcode() << '\n'; // for chip8Program test.rom, std::cout << std::hex usually omits leading zeroes, so it prints: 0xe0

    std::cout << "memory[0x200]: 0x"
              << std::hex // type cast to hex
              << static_cast<int>(chip8.memory_at(0x200))
              << '\n';
}