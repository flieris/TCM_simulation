#include <iostream>
#include <fstream>
#include <complex>
#include "generator.h"
#include "coder.h"
#include "channel.h"
#include "modulator.h"
#include "decoder.h"
#include <random>

using namespace std;

void calculateBitErrorRate(int input_data, int decoded_data, int &error_count, int &correct_count);

int main() {
   int error_count = 0, correct_count = 0;;
   Generator *source_ = new Generator(2);
   Coder *coder_ = new Coder();
   Modulator *modulator_ = new Modulator();
   Channel *channel = new Channel(9);
   Decoder *decoder = new Decoder();
   complex<double> check, modulated, noisy;
   unsigned int symbol, encoded_symbol, decoded_symbol;
   double noise_real, noise_imag;
   std::vector<int> entry_symbol, exit_symbol;
   int index = 0;
   cout << "Conducting test" << endl;
   while (error_count < 100) {
      index++;
      if (index > 10000) break;
      symbol = static_cast<int>(source_->Random(0, 4));
      cout << "entry now: " << symbol << endl;
      entry_symbol.push_back(symbol);
      encoded_symbol = coder_->encode(symbol);
      modulator_->modulate(encoded_symbol);
      modulated = modulator_->getOutputData();
      noisy = channel->noisify(modulated);
      //decoder->decode(modulated);
      decoder->decode(noisy);
      decoded_symbol = decoder->getDecodedData();
      cout << " exit now: " << decoder->getDecodedData() << endl;
      calculateBitErrorRate(symbol, decoded_symbol, error_count, correct_count);
   }
   double ber_ = (double)error_count / (index -1);
   cout << error_count << endl;
   cout << "Error count: " << ber_ << " After " << index << " loop iterations" << endl;
   delete source_;
   delete coder_;
   delete modulator_;
   delete decoder;
   system("pause");
}

void calculateBitErrorRate(int input_data, int decoded_data, int &error_count, int &correct_count) {
   unsigned int e_ = input_data ^ decoded_data;
   //cout << e_ << " elements: " << (e_ & 0x01) << " and " << (e_ & 0x02) << endl;
   if (e_ & 1 || e_ & 2) {
      error_count = error_count + 1;
   }
   else {
      correct_count = correct_count + 1;
   }
}