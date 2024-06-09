//
// Created by ozgur on 12/04/2024.
//

#include "../include/Instructions.h"
#include "../include/CPU.h"
#include <stdio.h>

namespace gameboy
{
    namespace instruction
    {
        Instructions::Instructions(CPU *_cpu) : cpuRef(_cpu)
        {
            standardInstructions =
                    {
                    };

            cbInstructions = {};
        }

        bool Instructions::init()
        {
            if (cpuRef == nullptr)
            {
                printf("CPU Ref could not be initialized in Instruction!!\n");
                return false;
            }

            return true;
        }
    }
}
