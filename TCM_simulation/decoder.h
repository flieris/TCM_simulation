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

   void initMemory();

   double getDistance(std::complex<double> data, int constelation_point);

private:
   // number of state constant indicates number of states(duh) and number of possible moves in one modulation cycle
   const int k_NUMBER_OF_STATES = 4;
   // arbitrary trellis depth - right now it's coder number of output bits (3) times 15 
   const int k_TRELLIS_DEPTH = 45;
   int trellis_index_;
   std::map<int, std::complex<double>> *_8psk_constelation;
   std::vector<std::vector<int>> next_state_;
   std::vector<std::vector<int>> output_state_;
   Path *trellis_memory_[4][45];
  // std::vector<Path*> *minimal_paths_;
   int decoded_data;
};

#endif // !decoder_h
