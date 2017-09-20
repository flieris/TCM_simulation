#include "channel.h"
#include <cmath>
#include <iostream>
using namespace std;
Channel::Channel(double snr) {
   variance = sqrt(2.0*(2.0 / 3))*pow(10.0, -snr / 10.0);
   dist_ = new  normal_distribution<double>(0.0, variance);
   generator_ = new default_random_engine;
}

Channel::~Channel() {

}

std::complex<double> Channel::noisify(std::complex<double> input) {

   double real_value = input.real() + (*dist_)(*generator_);
   double imag_value = input.imag() + (*dist_)(*generator_);
   return std::complex<double>(real_value, imag_value);
}

