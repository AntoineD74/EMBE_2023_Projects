#include <stdint.h> 


class Encoder
{
    private:
        int position;

    public: 
        Encoder();
        void updateCounter(bool clockwise);
        int getPosition();
};

Encoder::Encoder(): position(0)
{
    
}

void Encoder::updateCounter(bool clockwise)
{
    if(clockwise){ position++; }
    else{ position--; }
}

int Encoder::getPosition()
{
    return position;
}
