#include "channel.h"
#include <cmath>
#include <iostream>
using namespace std;
Channel::Channel(double snr) {
   variance = pow(10.0, (-snr / 10.0));
   noise_generator = new Generator(420);
}

Channel::~Channel() {

}

std::complex<double> Channel::noisify(std::complex<double> input) {
   double noise_real = noise_generator->Random() * variance * (1 / sqrt(variance));
   double noise_imag = noise_generator->Random() * variance * (1 / sqrt(variance));
   cout << "Noise values: " << endl << "real: " << noise_real << " imag: " << noise_imag << endl;
   double real_value = input.real() + noise_generator->Random(- variance, variance);
   double imag_value = input.imag() + noise_generator->Random(- variance, variance);
   return std::complex<double>(real_value, imag_value);
}

