#ifndef modulator_h
#define modulator_h
#include <complex>
#include <map>

class Modulator {
public:
   Modulator();
   ~Modulator();

   void modulate(unsigned int encoded_data);

   std::complex<double> getOutputData();
private:
   std::map<int,std::complex<double>> *_8psk_constellation;
   std::complex<double> output_data;
};
#endif // !modulator_h
