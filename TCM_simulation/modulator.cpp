#include "modulator.h"
#include <complex>

Modulator::Modulator() {
   _8psk_constelation = new std::map<int, std::complex<double>>;
   (*_8psk_constelation)[0] = std::complex<double>(1.0, 0.0);
   (*_8psk_constelation)[1] = std::complex<double>(0.707, 0.707);
   (*_8psk_constelation)[2] = std::complex<double>(0.0, 1.0);
   (*_8psk_constelation)[3] = std::complex<double>(-0.707, 0.707);
   (*_8psk_constelation)[4] = std::complex<double>(-1.0, 0.0);
   (*_8psk_constelation)[5] = std::complex<double>(-0.707, -0.707);
   (*_8psk_constelation)[6] = std::complex<double>(0.0, -1.0);
   (*_8psk_constelation)[7] = std::complex<double>(0.707, -0.707);
}
Modulator::~Modulator() {
   delete _8psk_constelation;
}

void Modulator::modulate(unsigned int encoded_data) {
   output_data = 0;

   output_data = _8psk_constelation->operator[](encoded_data);
}

std::complex<double> Modulator::getOutputData()
{
   return output_data;
}
