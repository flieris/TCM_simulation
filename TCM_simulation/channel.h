#ifndef channel_h
#define channel_h
#include "generator.h"
#include <complex>
#include <random>
class Channel {
public:
   Channel(double snr);
   ~Channel();

   std::complex<double> noisify(std::complex<double> input);
private:
   double variance;
   std::default_random_engine *generator_;
   std::normal_distribution<double> *dist_;
};
#endif // !channel_h
