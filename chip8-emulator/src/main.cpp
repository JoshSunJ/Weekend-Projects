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
        0x60, 0x0A, // V0 = 10
        0x61, 0x05, // V1 = 5
        0x80, 0x15, // V0 = V0 - V1; no borrow, VF = 1
        0x62, 0x03, // V2 = 3
        0x63, 0x05, // V3 = 5
        0x82, 0x35  // V2 = V2 - V3; borrow, VF = 0
    };

    // Write the CHIP-8 opcodes to the file
 

    rom_file.write(
        reinterpret_cast<const char*>(chip8_program), sizeof(chip8_program)
    );
    rom_file.close();

    chip8.load_rom("test.rom");


    for (int cycle = 0; cycle < 6; ++cycle) {
        std::cout << "Fetched opcode: 0x" << std::hex << chip8.cycle() << '\n';
    }

    std::cout << "PC after six cycles: 0x" << chip8.program_counter() << '\n';

    std::cout << "V0 after no-borrow subtraction: 0x"
              << static_cast<int>(chip8.register_at(0)) << '\n';

    std::cout << "V2 after borrow subtraction: 0x"
              << static_cast<int>(chip8.register_at(2)) << '\n';

    std::cout << "VF after borrow subtraction: 0x"
              << static_cast<int>(chip8.register_at(0xF)) << '\n';

}
