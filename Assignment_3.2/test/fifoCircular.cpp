#include "fifoCircular.h"

FifoCircular::FifoCircular()
{
    bufferHead = buffer;
    bufferTail = buffer;
    isFull = false;
}

int FifoCircular::get()
{
    if (this->is_empty()) {
        return 0;
    }

    int item = *bufferHead;
    bufferHead++;
    if (bufferHead == buffer + FIFOCIRCULAR_SIZE) {
        bufferHead = buffer;
    }

    isFull = false;
    return item;
}

void FifoCircular::put(int item)
{
    if (!this->is_full()) {
        *bufferTail = item;
        bufferTail++;
        if (bufferTail == buffer + FIFOCIRCULAR_SIZE) {
            bufferTail = buffer;
        }

        if (bufferHead == bufferTail) {
            isFull = true;
        }
    }
}

bool FifoCircular::is_empty()
{
    return (!isFull && bufferHead == bufferTail ? true: false);
}

bool FifoCircular::is_full()
{
    return isFull;
}

void FifoCircular::reset()
{
    bufferHead = buffer;
    bufferTail = buffer;
    isFull = false;
}