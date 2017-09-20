#include "coder.h"
#include <iostream>
using namespace std;
Coder::Coder() {
   state = 0;
}


Coder::~Coder() {

}

unsigned int Coder::encode(unsigned int data) {
   // calculate coder's first output bit
   unsigned int c0 = data >> 1;
   // calculate coder's second output bit
   unsigned int c1 = (state & 1) ^ (data & 1);
   // calculate coder's third output bit
   unsigned int c2 = (state >> 1);
   unsigned int c = (c0 << 2) | (c1 << 1) | c2;
   // update coder's state memory blocks
   state = (state >> 1) | ((data & 1) << 1);
   return c;
}

unsigned int Coder::getState()
{
   return state;
}

