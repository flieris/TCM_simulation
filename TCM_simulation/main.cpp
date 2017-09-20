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
void getSeeds(int(&tab)[10]);
void generateSeeds(int index_);
void calculateBitErrorRate(int input_data, int decoded_data, int &error_count, int &correct_count);

int main() {
   int sim_index = 0;
   std::cout << "Please input simulation index: [0-9] "; cin >> sim_index; std::cout << endl;
   int simnum = 0;
   fstream seeds_("seeds.txt", std::ios::in | std::ios::app);
   if (seeds_.peek() == std::fstream::traits_type::eof()) {
      generateSeeds(10);
      return 0;
   }
   seeds_.close();
   int seeds[10];
   getSeeds(seeds);
   for(simnum; simnum < 20; simnum++)
   {
      double snrr = simnum*0.5;
      std::cout << "Simulation number: " << simnum << " SNR value: " << snrr << endl;
      int error_count = 0, correct_count = 0;;
      Generator *source_ = new Generator(seeds[sim_index]); 
      Coder *coder_ = new Coder();
      Modulator *modulator_ = new Modulator();
      Channel *channel = new Channel(snrr);
      Decoder *decoder = new Decoder();
      complex<double> check, modulated, noisy;
      unsigned int symbol, encoded_symbol, decoded_symbol;
      int index = 0;
      std::cout << "Conducting test" << endl;
      while (error_count < 100) {
         index++;
         symbol = static_cast<int>(source_->Random(0, 4));
         encoded_symbol = coder_->encode(symbol);
         modulator_->modulate(encoded_symbol);
         modulated = modulator_->getOutputData();
         noisy = channel->noisify(modulated);
         decoder->decode(noisy);
         decoded_symbol = decoder->getDecodedData();
         calculateBitErrorRate(symbol, decoded_symbol, error_count, correct_count);
      }
      double ber_ = (double)error_count / (error_count + correct_count);
      std::cout << "Error count: " << ber_ << " After " << index << " loop iterations" << endl;
      delete source_;
      delete coder_;
      delete modulator_;
      delete channel;
      delete decoder;
   }

   system("pause");
}

void calculateBitErrorRate(int input_data, int decoded_data, int &error_count, int &correct_count) {
   unsigned int e_ = input_data ^ decoded_data;
   if (e_ & 1 || e_ & 2) {
      error_count = error_count + 1;
   }
   else {
      correct_count = correct_count + 1;
   }
}

void generateSeeds(int simulations) {
   int value;
   fstream seeds_("seeds.txt", std::ios::out | std::ios::trunc);

   Generator *test_ = new Generator(123);
   int seeds = simulations;
   int number = 20000 / seeds;
   for (int i = 0; i < 20000; ++i) {
      value = static_cast<int>(test_->Random(0, 20000));
      if (i % number == 0)
      {
         seeds_ << value << " ";
      }
   }
   seeds_.close();
   delete test_;
}

void getSeeds(int (&tab)[10]) {
   fstream seeds_file("seeds.txt", std::ios::in);
   for (int i = 0; i < 10; ++i)
   {
      seeds_file >> tab[i];
   }
   seeds_file.close();
}