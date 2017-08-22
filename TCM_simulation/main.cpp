#include <iostream>
#include <fstream>
#include "generator.h"
#include "coder.h"
#include "modulator.h"


using namespace std;

int main() {

   Generator *source_ = new Generator(123);
   Coder *coder_ = new Coder();
   Modulator *modulator_ = new Modulator();
   unsigned int symbol, encoded_symbol;
   fstream test_file("test.txt", std::ios::out | std::ios::binary | std::ios::trunc);
   cout << "Conducting test" << endl;
   cout << (2 & 1) << endl;
   for (int i = 0; i < 100; ++i) {
      symbol = static_cast<int>(source_->Random(0,4));
      cout << "Symbol = " << symbol << endl;
      encoded_symbol = coder_->encode(symbol);
      cout << "Encoded symbol = "<< encoded_symbol << endl;
      modulator_->modulate(encoded_symbol);
      cout << "Symbol after 8PSK modulation = " << modulator_->getOutputData() << endl;
   }
   cout << symbol;
   test_file.close();
   system("pause");
}