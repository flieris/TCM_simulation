#ifndef decoder_h
#define decoder_h
#include "path.h"
#include <complex>
#include <map>
#include <array>
#include <vector>


class Decoder {
public:
   Decoder();
   ~Decoder();

   void decode(std::complex<double> input_data);
   int getDecodedData();
   double getDistance(std::complex<double> data, int constelation_point);

private:
   // number of state constant indicates number of states(duh) and number of possible moves in one modulation cycle
   const int num_of_states_ = 4;
   std::map<int, std::complex<double>> *_8psk_constellation;
   std::vector<std::vector<int>> next_state_;
   std::vector<std::vector<int>> output_state_;
   std::vector<std::vector<Path*>>trellis_;
   int decoded_data;
};

#endif // !decoder_h
