#include "transmitter.h"
#include <iostream>
using namespace std;
Transmitter::Transmitter() {
   state = 0;
}


Transmitter::~Transmitter() {

}


unsigned int Transmitter::encode(unsigned int data) {
   unsigned int c0 = data >> 1;
   unsigned int c1 = (state & 1) ^ (data & 1);
   unsigned int c2 = c1 ^ (state >> 1);
   
   unsigned int c = (c0 << 2) | (c1 << 1) | c2;
   state = (state >> 1) | ((data & 1) << 1);
   cout << state << endl;
   return c;
}

complex<double> Transmitter::modulator(unsigned int encoded_data) {
   //<--    TODO     -->>//
   //Add modulator functionality 


}