#include "channel.h"
#include <cmath>
#include <iostream>
Channel::Channel(double snr) {
   variance = pow(10.0, (-snr / 10.0));
   noise_generator = new Generator(420);
}

Channel::~Channel() {

}

std::complex<double> Channel::noisify(std::complex<double> input) {
   double real_value = input.real() + noise_generator->Random(- variance, variance);
   double imag_value = input.imag() + noise_generator->Random(- variance, variance);
   return std::complex<double>(real_value, imag_value);
}

