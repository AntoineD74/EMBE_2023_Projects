#include <fifo.h>

// Fifo::Fifo()
// {
//     bufferFront = &buffer[0];
//     bufferBack = bufferFront;
// }

// int Fifo::get()
// {
//     if(!this->is_empty())
//     {
//         int fo = buffer[0];
//         for(int i=0; i < (bufferBack - bufferFront -1); i++){
//             buffer[i] = buffer[i+1];
//         }
//         bufferBack -= 1;
//         return fo;
//     }
//     else{
//         return -1;
//     }
// }

// void Fifo::put(int item)
// {
//     if(!this->is_full())
//     {
//         *bufferBack = item;
//         bufferBack += 1;
//     }
// }

// bool Fifo::is_empty()
// {
//     return (bufferBack - bufferFront == 0 ? true : false);
// }

// bool Fifo::is_full()
// {
//     return (bufferBack - bufferFront == 5 ? true : false);
// }

// void Fifo::reset()
// {
//     bufferBack = &buffer[0];
// }

// int Fifo::getContent(int i)
// {
//     return buffer[i];
// }

#include <fifo.h>

Fifo::Fifo()
{

}

int Fifo::get()
{
    return 0;
}

void Fifo::put(int item)
{

}

bool Fifo::is_empty()
{
    return false;
}

bool Fifo::is_full()
{
    return false;
}

void Fifo::reset()
{

}