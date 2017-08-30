#include <iostream>
#include <fstream>
#include "generator.h"
#include "coder.h"
#include "modulator.h"
#include "decoder.h"

using namespace std;

int main() {

   Generator *source_ = new Generator(123);
   Coder *coder_ = new Coder();
   Modulator *modulator_ = new Modulator();
   Decoder *decoder = new Decoder();
   unsigned int symbol, encoded_symbol;
   std::vector<double> modulated_symbol;
   fstream test_file("test.txt", std::ios::out | std::ios::binary | std::ios::trunc);
   cout << "Conducting test" << endl;
   cout << (2 & 1) << endl;
   for (int i = 0; i < 10; ++i) {
      symbol = static_cast<int>(source_->Random(0,4));
      cout << "Symbol = " << symbol << endl;
      encoded_symbol = coder_->encode(symbol);
      cout << "Encoded symbol = "<< encoded_symbol << endl;
      modulator_->modulate(encoded_symbol);
      cout << "Symbol after 8PSK modulation = " << modulator_->getOutputData() << endl;
      decoder->decode(modulator_->getOutputData());
      //cout << "Decoded symbol = " << decoder->getDecodedData() << endl;
   }
   cout << symbol;
   test_file.close();
   delete source_;
   delete coder_;
   delete modulator_;
   delete decoder;
   system("pause");
}