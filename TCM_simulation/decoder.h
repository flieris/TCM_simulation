#ifndef decoder_h
#define decoder_h
#include "path.h"
#include <complex>
#include <map>
#include <vector>


class Decoder {
public:
   Decoder();
   ~Decoder();

   void decode(std::complex<double> input_data);
   int getDecodedData();

   void initMemory();

   double getDistance(std::complex<double> data, int constelation_point);

private:
   std::map<int, std::complex<double>> *_8psk_constelation;
   std::vector<std::vector<int>> next_state_;
   std::vector<std::vector<int>> output_state_;
   std::vector<std::vector<Path*>*>* trellis_memory_;
  // std::vector<Path*> *minimal_paths_;
   int decoded_data;
};

#endif // !decoder_h
