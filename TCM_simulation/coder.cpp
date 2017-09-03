#include "coder.h"
#include <iostream>
using namespace std;
Coder::Coder() {
   state = 0;
}


Coder::~Coder() {

}


unsigned int Coder::encode(unsigned int data) {
   unsigned int c0 = data >> 1;
   unsigned int c1 = (state & 1) ^ (data & 1);
   unsigned int c2 = c1 ^ (state >> 1);
   
   unsigned int c = (c0 << 2) | (c1 << 1) | c2;
   state = (state >> 1) | ((data & 1) << 1);
   //cout << state << endl;
   return c;
}

unsigned int Coder::getState()
{
   return state;
}

