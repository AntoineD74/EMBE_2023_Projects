#include <stdint.h> 


class Encoder
{
    private:
        uint8_t pinC1;
        uint8_t pinC2;
        int position;

    public: 
        Encoder(int pin1, int pin2);
        void init();
        bool is_C1_hi();
        bool is_C2_hi();
        void updateCounter(bool clockwise);
        int getPosition();
};