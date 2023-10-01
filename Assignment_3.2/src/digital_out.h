#include <stdint.h> 

class Digital_out  
{
    private:
        uint8_t pinMask;

    public:
        Digital_out(uint8_t portNumber);
        void init();
        void set_hi();
        void set_lo();
        void toggle();
};