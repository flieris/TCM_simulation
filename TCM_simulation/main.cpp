#include <iostream>
#include <fstream>
#include <complex>
#include "generator.h"
#include "coder.h"
#include "channel.h"
#include "modulator.h"
#include "decoder.h"

using namespace std;


int main() {
   Generator *source_ = new Generator(123);
   Coder *coder_ = new Coder();
   Modulator *modulator_ = new Modulator();
   Channel *channel = new Channel(15);
   Decoder *decoder = new Decoder();
   complex<double> check, modulated, noisy;
   unsigned int symbol, encoded_symbol, decoded_symbol;
   double noise_real, noise_imag;
   std::vector<int> entry_symbol, exit_symbol;
   
   cout << "Conducting test" << endl;
   for (int i = 0; i < 5; ++i) {
      symbol = static_cast<int>(source_->Random(0,4));
      entry_symbol.push_back(symbol);
      cout << "Symbol = " << symbol << endl;
      encoded_symbol = coder_->encode(symbol);
      cout << "Encoded symbol = "<< encoded_symbol << endl;
      modulator_->modulate(encoded_symbol);
      modulated = modulator_->getOutputData();
      //noisy = channel->noisify(modulated);
      //cout << "Data after noisy channel: " << noisy << endl;
      cout << "Symbol after 8PSK modulation = " << modulator_->getOutputData() << endl;
      decoder->decode(modulated);
      decoded_symbol = decoder->getDecodedData();
      exit_symbol.push_back(decoded_symbol);
      cout << "Decoded symbol = " << decoder->getDecodedData() << endl;
   }
   fstream test_file("test.txt", std::ios::out | std::ios::binary | std::ios::trunc);
   for (int i = 0; i < entry_symbol.size(); i++) {
      test_file << "Entry symbol: " << entry_symbol[i];
      test_file << " Exit symbol: " << exit_symbol[i];
      test_file << endl;
   }
   test_file.close();
   delete source_;
   delete coder_;
   delete modulator_;
   delete decoder;
   system("pause");
}