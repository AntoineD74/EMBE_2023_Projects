#ifndef FIFOCIRCULAR_H
#define FIFOCIRCULAR_H

const int FIFOCIRCULAR_SIZE = 5;

class FifoCircular
{
    public:
        FifoCircular();
        int get();
        void put(int item);
        bool is_empty();
        bool is_full();
        void reset();
        int getContent(int i);
        
    private:
        int buffer[FIFOCIRCULAR_SIZE];
        int* bufferTail;
        int* bufferHead;
        bool isFull;
};

#endif