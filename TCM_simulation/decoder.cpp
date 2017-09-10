#include "decoder.h"
#include <complex>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <array>

Decoder::Decoder() {
   _8psk_constelation = new std::map<int, std::complex<double>>;
   _8psk_constelation->operator[](0) = std::complex<double>(1.0, 0.0);
   _8psk_constelation->operator[](1) = std::complex<double>(0.707, 0.707);
   _8psk_constelation->operator[](2) = std::complex<double>(0.0, 1.0);
   _8psk_constelation->operator[](3) = std::complex<double>(-0.707, 0.707);
   _8psk_constelation->operator[](4) = std::complex<double>(-1.0, 0.0);
   _8psk_constelation->operator[](5) = std::complex<double>(-0.707, -0.707);
   _8psk_constelation->operator[](6) = std::complex<double>(0.0, -1.0);
   _8psk_constelation->operator[](7) = std::complex<double>(0.707, -0.707);
   decoded_data = new std::vector<int>;
   
   // trellis info:
   // next state
   next_state_.push_back({ 0,0,1,1 });
   next_state_.push_back({ 2,2,3,3 });
   next_state_.push_back({ 0,0,1,1 });
   next_state_.push_back({ 2,2,3,3 });
   // output state
   output_state_.push_back({ 0,4,2,6 });
   output_state_.push_back({ 1,5,3,7 });
   output_state_.push_back({ 2,6,0,4 }); 
   output_state_.push_back({ 3,7,1,5 });
   // input state
   input_state_.push_back({ 0,2,1,3 });
   input_state_.push_back({ 0,2,1,3 });
   input_state_.push_back({ 0,2,1,3 });
   input_state_.push_back({ 0,2,1,3 });
   // begin state
   begin_state_.push_back({ 0,0,0,0 });
   begin_state_.push_back({ 1,1,1,1 });
   begin_state_.push_back({ 2,2,2,2 });
   begin_state_.push_back({ 3,3,3,3 });
   // create trellis vector
   for (int i = 0; i < num_of_states_; ++i) {
      trellis_.push_back(std::vector<Path>());
   }

   for (int i = 0; i < num_of_states_; ++i) {
      metrics_.push_back(9999.0);
   }
   metrics_[0] = 0.0;
}

Decoder::~Decoder() {
   delete _8psk_constelation;
}

void Decoder::decode(std::vector<std::complex<double>> *input_stream) {
   //calculate Euclidian distances between input signal and every point on constellation
   for (int index = 0; index < input_stream->size(); ++index) {
      std::complex<double> input_data = input_stream->operator[](index);
      auto distances = new std::vector<double>;
      for (int i = 0; i < 8; i++) {
         distances->push_back(getDistance(input_data, i));
      }
      for (int from_state = 0; from_state < num_of_states_; ++from_state) {
         int minimal_path_index = 0;
         double minimal_path_cost = (*distances)[output_state_[from_state][0]] +
            metrics_[begin_state_[from_state][0]];
         for (int input_state = 0; input_state < num_of_states_; ++input_state) {
            double cost = metrics_[begin_state_[from_state][input_state]];
            cost += (*distances)[output_state_[from_state][input_state]];
            if (cost < minimal_path_cost) {
               minimal_path_cost = cost;
               minimal_path_index = input_state;
            }
         }
         Path new_path;
         new_path.input_ = input_state_[from_state][minimal_path_index];
         new_path.metric_ = minimal_path_cost;
         new_path.previous_ = begin_state_[from_state][minimal_path_index];
         trellis_[from_state].push_back(new_path);

         if (!(index < trellis_depth_)) {
            trellis_[from_state].erase(trellis_[from_state].begin());
         }
      }

      for (int i = 0; i < num_of_states_; ++i) {
         metrics_[i] = trellis_[i].back().metric_;
      }
      if (index < trellis_depth_ - 1) {
         continue;
      }
      int minimal_index = 0;
      double minimal_cost = metrics_[0];
      for (int i = 1; i < num_of_states_; ++i) {
         if (metrics_[i] < minimal_cost) {
            minimal_cost = metrics_[i];
            minimal_index = i;
         }
      }
      int current = minimal_index;
      for (int i = trellis_[minimal_index].size() - 1; i > 0; --i) {
         current = trellis_[current][i].previous_;
      }
      int temp = index - trellis_depth_ + 2;
      (*decoded_data).push_back(trellis_[current].front().input_);
      //std::reverse(std::begin(*decoded_data), std::end(*decoded_data));
   }
}

std::vector<int>* Decoder::getDecodedData() {
   return decoded_data;
}

double Decoder::getDistance(std::complex<double> data, int constelation_point){
   double delta_x = data.real() - _8psk_constelation->operator[](constelation_point).real();
   double delta_y = data.imag() - _8psk_constelation->operator[](constelation_point).imag();
   double result = floorf(abs(std::complex<double>(delta_x, delta_y))*100)/100;
   //std::cout << "distance: " << result << std::endl;
   return pow(result,2.0);
   //return abs(std::complex<double>(delta_x, delta_y));
}