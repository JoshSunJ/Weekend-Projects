#include <array>
#include <cstdint>
#include <filesystem>

// Do not put `using namespace std;` in a header.

class Chip8 {
    public:
        Chip8();                       // Exact spelling: Chip8, not Cihp8
        void reset();                  // reset does not return a value
        std::uint16_t program_counter() const; // program_counter() const is a public “getter”: it lets main.cpp read the PC without exposing the actual variable for modification.
        void load_rom(const std::filesystem::path& path);
        std::uint8_t memory_at(std::uint16_t address) const; //const makes the function read-only, members variables can't be changed
        std::uint16_t fetch_opcode() const;
        std::uint16_t cycle();


    // Keep your constants and all state members here.
    private:
    //initialize constant sizes
        static constexpr std::size_t memory_size = 4096;
        static constexpr std::size_t register_count = 16;
        static constexpr std::size_t stack_depth = 16;
        static constexpr std::size_t key_count = 16;
        static constexpr std::size_t display_width = 64; //When placed inside a function, static extends the variable's lifetime to the entire duration of the program
        static constexpr std::size_t display_height = 32; // This keyword indicates that the variable is a compile-time constant. The value must be known at compile time, allowing for optimizations and ensuring that the variable can be used in constant expressions.

        static constexpr std::uint16_t program_start = 0x200;

        //array values
        std::array<std::uint8_t, memory_size> memory_;
        std::array<std::uint8_t, register_count> registers_;
        std::array<std::uint16_t, stack_depth> stack_;
        std::array<std::uint8_t, key_count> keypad_;
        std::array<std::uint8_t, display_width * display_height> display_;

        //scalar values
        std::uint16_t index_;
        std::uint16_t program_counter_;
        std::uint8_t stack_pointer_;
        std::uint8_t delay_timer_;
        std::uint8_t sound_timer_;
};


