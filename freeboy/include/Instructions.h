//
// Created by ozgur on 3/04/2024.
//

#ifndef FREEBOY_Info_H
#define FREEBOY_Info_H

#include <cstdint>
#include <array>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace gameboy {

    class CPU;

    namespace instruction {

        enum class ConditionCode : uint8_t {
            NONE,   // Nothing
            NC,     // Execute if C is not set.
            C,      // Execute if C is set.
            NZ,     // Execute if Z is not set.
            Z       // Execute if Z is set.
        };

        enum class RegisterType : uint8_t {
             NONE,
             A,
             F,
             B,
             C,
             D,
             E,
             H,
             L,
             AF,
             BC,
             DE,
             HL,
             SP,
             PC
        };

        class Instructions
        {
        public:
            struct Info
            {
                uint8_t length = 0;
                uint8_t tCycles = 0;
                void (gameboy::CPU::*operation)();
                void (gameboy::CPU::*addrMode)();
                RegisterType dstRegister = RegisterType::NONE;
                RegisterType srcRegister = RegisterType::NONE;
                ConditionCode condition = ConditionCode::NONE;
                uint8_t param{0u};
            };

            Instructions(CPU* _cpu);

            bool init();

            std::array<Info, 0x100> standardInstructions;
            std::array<Info, 0x100> cbInstructions;
            std::vector<std::string_view> lookUp;
        private:
            CPU* cpuRef;
        };
    }
}
#endif //FREEBOY_Info_H
