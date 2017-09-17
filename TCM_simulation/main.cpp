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
   double variance1 = sqrt(2.0*(2.0 / 3))*pow(10.0, -1.0 / 10.0);
   double variance2 = sqrt(2.0*(2.0 / 3))*pow(10.0, -20 / 10.0);
   cout << "variance for snr 10: " << variance1 << "variance for snr 20: " << variance2 << endl;
   normal_distribution<double> *dist1_ = new  normal_distribution<double>(0.0, variance1);
   normal_distribution<double> *dist2_ = new  normal_distribution<double>(0.0, variance2);
   default_random_engine *generator_ = new default_random_engine();
   //Generator* gen_ = new Generator(420);
   for (int i = 0; i < 4; ++i) {
      cout << "snr: 10 " << (*dist1_)(*generator_) << endl;
      cout << "snr: 20 " << (*dist2_)(*generator_) << endl;
   }
   int error_count = 0, correct_count = 0;;
   Generator *source_ = new Generator(123);
   Coder *coder_ = new Coder();
   Modulator *modulator_ = new Modulator();
   Channel *channel = new Channel(25);
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
      cout << "entry now: " << symbol;
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
      // TODO: zaimplementowaæ opóŸnienie decyzji
      //if (index > 15) {
         //cout << index << endl;
     //    decoded_symbol = decoder->getDecodedData(index - 16);
     //    exit_symbol.push_back(decoded_symbol);
      //   cout << "entry: " << entry_symbol[index - 16] << " exit: " << exit_symbol.back() << endl;
     //    calculateBitErrorRate(entry_symbol[index-16], exit_symbol.back(), error_count, correct_count);
     // }
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