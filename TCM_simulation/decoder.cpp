#include "decoder.h"
#include <complex>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
Decoder::Decoder() {
   _8psk_constelation = new map<int, complex<double>>;
   _8psk_constelation->operator[](0) = complex<double>(1.0, 0.0);
   _8psk_constelation->operator[](1) = complex<double>(0.707, 0.707);
   _8psk_constelation->operator[](2) = complex<double>(0.0, 1.0);
   _8psk_constelation->operator[](3) = complex<double>(-0.707, 0.707);
   _8psk_constelation->operator[](4) = complex<double>(-1.0, 0.0);
   _8psk_constelation->operator[](5) = complex<double>(-0.707, -0.707);
   _8psk_constelation->operator[](6) = complex<double>(0.0, -1.0);
   _8psk_constelation->operator[](7) = complex<double>(0.707, -0.707);
   decoded_data = 0;
   
   // trellis info:
   // next state
   next_state_.push_back({ 0,1,0,1 });
   next_state_.push_back({ 2,3,2,3 });
   next_state_.push_back({ 0,1,0,1 });
   next_state_.push_back({ 2,3,2,3 });
   // output state
   output_state_.push_back({ 0,2,4,6 });
   output_state_.push_back({ 1,3,5,7 });
   output_state_.push_back({ 2,0,6,4 }); 
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
   for (int i = 0; i < num_of_states_; ++i)
      trellis_.push_back(vector<Path*>());

   for (int i = 0; i < num_of_states_; ++i) {
      metrics_.push_back(9999.0);
   }
   metrics_[0] = 0.0;
}

Decoder::~Decoder() {
   delete _8psk_constelation;
}

void Decoder::decode(complex<double> input_data) {
   //calculate Euclidian distances between input signal and every point on constellation
   auto distances = new vector<double>;
   for (int i = 0; i < 8; i++) {
      distances->push_back(getDistance(input_data, i));
   }
   for (int from_state = 0; from_state < num_of_states_; ++from_state) {
      vector<Path*> *metrics = new vector<Path*>;
      metrics->clear();
      for (int i = 0; i < 4; i++) {
         metrics->push_back(nullptr);
      }
      Path *minimal_path = new Path();
      for (int input_state = 0; input_state < num_of_states_; ++input_state) {
         Path *new_path = new Path();
         new_path->input_ = input_state;
         new_path->previous_ = from_state;
         new_path->next_ = next_state_[from_state][input_state];
         double output_ = output_state_[from_state][input_state];
         cout << output_ << endl;
         new_path->metric_ = getDistance(input_data, output_);
         if (trellis_[from_state].size() > 0) {
            new_path->metric_ += trellis_[from_state].back()->metric_;
         }
         if ((*metrics)[input_state] == nullptr) {
            (*metrics)[input_state] = new_path;
         }
      }
       int min_index = 0;
       for (int i = 1; i < num_of_states_; ++i) {
          if ((*metrics)[i]->metric_ < (*metrics)[min_index]->metric_) {
             min_index = i;
         }
      }
      minimal_path = (*metrics)[min_index];
      trellis_[from_state].push_back(minimal_path);
   }
   for (int i = 0; i < trellis_.size(); ++i) {
      for (int j = 0; j < trellis_[i].size(); ++j) {
         cout << "Path stats: from state: " << trellis_[i][j]->previous_ << " to state: " << trellis_[i][j]->next_ << " input: " << trellis_[i][j]->input_ << " metric: " << trellis_[i][j]->metric_ << endl;
      }
   }
}

int Decoder::getDecodedData() {

   return decoded_data;
}

double Decoder::getDistance(complex<double> data, int constelation_point){
   double delta_x = data.real() - _8psk_constelation->operator[](constelation_point).real();
   double delta_y = data.imag() - _8psk_constelation->operator[](constelation_point).imag();
   //double result = floorf(abs(complex<double>(delta_x, delta_y))*100)/100;
   //std::cout << "distance: " << result << std::endl;
   //return pow(result,2.0);
   return abs(complex<double>(delta_x, delta_y));
}
/*
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
begin_state_.push_back({ 3,3,3,3 });*/