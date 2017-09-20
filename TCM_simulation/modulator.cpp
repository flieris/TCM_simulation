#include "modulator.h"
#include <complex>
#include <math.h>
# define M_PI           3.14159265358979323846

Modulator::Modulator() {
   // 8psk constellation initialization 
   _8psk_constellation = new std::map<int, std::complex<double>>;
   (*_8psk_constellation)[0] = std::complex<double>(1.0, 0.0);
   (*_8psk_constellation)[1] = std::complex<double>(cos(M_PI / 4.0), sin(M_PI / 4.0));
   (*_8psk_constellation)[2] = std::complex<double>(0.0, 1.0);
   (*_8psk_constellation)[3] = std::complex<double>(-cos(M_PI / 4.0), sin(M_PI / 4.0));
   (*_8psk_constellation)[4] = std::complex<double>(-1.0, 0.0);
   (*_8psk_constellation)[5] = std::complex<double>(-cos(M_PI / 4.0), -sin(M_PI / 4.0));
   (*_8psk_constellation)[6] = std::complex<double>(0.0, -1.0);
   (*_8psk_constellation)[7] = std::complex<double>(cos(M_PI / 4.0), -sin(M_PI / 4.0));
}
Modulator::~Modulator() {
   delete _8psk_constellation;
}

void Modulator::modulate(unsigned int encoded_data) {
   output_data = 0;
   // modulated symbol is basically complex number that corresponds to position on constellation 
   output_data = _8psk_constellation->operator[](encoded_data);
}

std::complex<double> Modulator::getOutputData()
{
   return output_data;
}
