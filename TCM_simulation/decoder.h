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

   void decode(std::vector<std::complex<double>> *input_stream);
   std::vector<int>* getDecodedData();

   double getDistance(std::complex<double> data, int constelation_point);

private:
   // number of state constant indicates number of states(duh) and number of possible moves in one modulation cycle
   const int num_of_states_ = 4;
   // arbitrary trellis depth - right now it's coder number of output bits (3) times 15 
   const int trellis_depth_ = 15;
   std::map<int, std::complex<double>> *_8psk_constelation;
   std::vector<std::vector<int>> next_state_;
   std::vector<std::vector<int>> output_state_;
   std::vector<std::vector<int>> input_state_;
   std::vector<std::vector<int>> begin_state_;
   std::vector<std::vector<Path>> trellis_;
   std::vector<double> metrics_;
  // std::vector<Path*> *minimal_paths_;

   std::vector<int>* decoded_data;
};

#endif // !decoder_h
