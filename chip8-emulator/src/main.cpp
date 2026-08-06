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
    uint8_t chip8Program[] = {
        0x00, 0xE0, // Clear the display
        0x61, 0x05, // Set V0 to 5
        0x61, 0x0A, // Set V1 to 10
        0x82, 0x02, // Add V1 to V0
        0xD0, 0x12  // Draw sprite at (X=0, Y=0)
    };

    rom_file.write(
        reinterpret_cast<const char*>(chip8Program), sizeof(chip8Program)
    );
    rom_file.close();

    chip8.load_rom("test.rom");

    std::cout << "memory[0x200]: 0x"
              << std::hex // type cast to hex
              << static_cast<int>(chip8.memory_at(0x200))
              << '\n';
}