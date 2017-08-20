#ifndef transmitter_h
#define transmitter_h
#include <complex>
using namespace std;
class Transmitter {
public:
   Transmitter();
   ~Transmitter();

   unsigned int encode(unsigned int data);

   complex<double> modulator(unsigned int coded_data);

private:
   unsigned int state;
   complex<double> modulated_data;
};

#endif // !transmitter_h




