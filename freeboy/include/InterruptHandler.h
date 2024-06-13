//
// Created by ozgur on 6/13/2024.
//

#ifndef FREEBOY_INTERRUPTHANDLER_H
#define FREEBOY_INTERRUPTHANDLER_H

namespace gameboy
{
    class InterruptHandler {
    public:
        InterruptHandler() : IME(false) {}

        void setIME(bool _isEnable) { IME = _isEnable; };
    private:
        bool IME;
    };
}



#endif //FREEBOY_INTERRUPTHANDLER_H
