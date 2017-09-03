#ifndef channel_h
#define channel_h
#include "generator.h"
#include <complex>
class Channel {
public:
   Channel(double snr);
   ~Channel();

   std::complex<double> noisify(std::complex<double> input);
private:
   double variance;
   Generator *noise_generator;
};
#endif // !channel_h
