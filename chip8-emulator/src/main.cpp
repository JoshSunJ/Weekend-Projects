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
        reinterpret_cast<const char*>(chip8_program), sizeof(chip8_program)
    );
    rom_file.close();

    chip8.load_rom("test.rom");


    const auto first_opcode = chip8.cycle();

    std::cout << "Fetched first opcode: 0x" << std::hex << first_opcode << '\n';

    std::cout << "PC after first cycle: 0x" << chip8.program_counter() << '\n';

    const auto second_opcode = chip8.cycle();

    std::cout << "Fetched second opcode: 0x" << std::hex << second_opcode << '\n';

    std::cout << "PC after second cycle: 0x" << chip8.program_counter() << '\n';

    std::cout << "V0 after two cycles: 0x"
              << static_cast<int>(chip8.register_at(0)) << '\n';

    std::cout << "V1 after two cycles: 0x"
              << static_cast<int>(chip8.register_at(1)) << '\n';
}
