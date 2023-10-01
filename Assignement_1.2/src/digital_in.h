#include <stdint.h> 

class Digital_in
{
    private:
        uint8_t pinMask;

    public:
        Digital_in(uint8_t pinNumber);
        void init();
        bool is_hi();
        bool is_lo();
};