#ifndef FIFO_H
#define FIFO_H

const int FIFO_SIZE = 5;

class Fifo
{
    public:
        Fifo();
        int get();
        void put(int item);
        bool is_empty();
        bool is_full();
        void reset();
        int getContent(int i);
        
        int* bufferBack;
        int* bufferFront;
    private:
        int buffer[FIFO_SIZE];
         
};

#endif // FIFO_H