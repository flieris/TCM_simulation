#include "decoder.h"
#include <complex>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
# define M_PI           3.14159265358979323846
using namespace std;
Decoder::Decoder() {
   _8psk_constelation = new map<int, complex<double>>;
   _8psk_constelation->operator[](0) = complex<double>(1.0, 0.0);
   _8psk_constelation->operator[](1) = complex<double>(cos(M_PI / 4.0), sin(M_PI / 4.0));
   _8psk_constelation->operator[](2) = complex<double>(0.0, 1.0);
   _8psk_constelation->operator[](3) = complex<double>(-cos(M_PI / 4.0), sin(M_PI / 4.0));
   _8psk_constelation->operator[](4) = complex<double>(-1.0, 0.0);
   _8psk_constelation->operator[](5) = complex<double>(-cos(M_PI / 4.0), -sin(M_PI / 4.0));
   _8psk_constelation->operator[](6) = complex<double>(0.0, -1.0);
   _8psk_constelation->operator[](7) = complex<double>(cos(M_PI / 4.0), -sin(M_PI / 4.0));
   decoded_data = 0;
   
   // trellis info:
   // next state
   output_state_.push_back({ 0,2,4,6 });
   output_state_.push_back({ 2,0,6,4 });
   output_state_.push_back({ 1,3,5,7 });
   output_state_.push_back({ 3,1,7,5 });

   next_state_.push_back({ 0,2,0,2 });
   next_state_.push_back({ 0,2,0,2 });
   next_state_.push_back({ 1,3,1,3 });
   next_state_.push_back({ 1,3,1,3 });
   /*
   next_state_.push_back({ 0,1,0,1 });
   next_state_.push_back({ 2,3,2,3 }); 
   next_state_.push_back({ 0,1,0,1 }); 
   next_state_.push_back({ 2,3,2,3 });
   // output state
   output_state_.push_back({ 0,2,4,6 });
   output_state_.push_back({ 1,3,5,7 }); 
   output_state_.push_back({ 2,0,6,4 }); 
   output_state_.push_back({ 3,1,7,5 });*/
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
   int pathways[4] = { 0,0,0,0 };
   vector<Path*> *path_metrics = new vector<Path*>;
   path_metrics->clear();
   for (int from_state = 0; from_state < num_of_states_; ++from_state) {
      vector<Path*> *state_metrics = new vector<Path*>;
      state_metrics->clear();
      for (int i = 0; i < 4; i++) {
         state_metrics->push_back(nullptr);
      }
      Path *minimal_path = new Path();
      for (int input_state = 0; input_state < num_of_states_; ++input_state) {
         int next = next_state_[from_state][input_state];
         Path *new_path = new Path();
         new_path->input_ = input_state;
         new_path->previous_ = from_state;
         new_path->next_ = next;
         double output_ = output_state_[from_state][input_state];
         new_path->metric_ = getDistance(input_data, output_);
         if (trellis_[from_state].size() > 0) {
            double cost = new_path->metric_ + trellis_[from_state].back()->metric_;
            new_path->metric_ = cost;
         }
         if ((*state_metrics)[input_state] == nullptr) {
            (*state_metrics)[input_state] = new_path;
         }
         else {
            if ((*state_metrics)[input_state]->metric_ > new_path->metric_) {
               (*state_metrics)[input_state] = new_path;
            }
         }
      }
       int min_index = 0;
       /*
       for (int j = 0; j < num_of_states_; ++j) {
          if (trellis_[(*state_metrics)[j]->next_].size() > 0) {
             double cost = (*state_metrics)[j]->metric_ + trellis_[from_state].back()->metric_;
             (*state_metrics)[j]->metric_ = cost;
          }
       }*/
       for (int i = 0; i < num_of_states_; ++i) {
          if ((*state_metrics)[i]->metric_ < (*state_metrics)[min_index]->metric_) {
             min_index = i;
         }
      }
      minimal_path = (*state_metrics)[min_index];
      path_metrics->push_back(minimal_path);
      pathways[from_state] = minimal_path->next_;

      //trellis_[from_state].push_back(minimal_path);
   }
   for (int i = 0; i < num_of_states_; ++i) {
      trellis_[pathways[i]].push_back(path_metrics->operator[](i));
   }
   /*
   for (int i = 0; i < trellis_.size(); ++i) {
      cout << "Path stats: from state: " << trellis_[i].back()->previous_ << " to state: " << trellis_[i].back()->next_ << " input: " << trellis_[i].back()->input_ << " metric: " << trellis_[i].back()->metric_ << endl;
     
   }*/
}

int Decoder::getDecodedData() {
   int min_index = 0;
   for (int i = 0; i < trellis_.size(); ++i) {
      if (trellis_[min_index].back()->metric_ > trellis_[i].back()->metric_) {
         min_index = i;
      }
   }
   decoded_data = trellis_[min_index].back()->input_;
   return decoded_data;
}

int Decoder::getDecodedData(int offset_) {
   int min_index = 0;
   for (int i = 0; i < trellis_.size(); ++i) {
      if (trellis_[min_index].back()->metric_ > trellis_[i].back()->metric_) {
         min_index = i;
      }
   }
   decoded_data = trellis_[min_index][offset_]->input_;
   return decoded_data;
}
double Decoder::getDistance(complex<double> data, int constelation_point){
   double delta_x =  _8psk_constelation->operator[](constelation_point).real() - data.real();
   double delta_y =  _8psk_constelation->operator[](constelation_point).imag() - data.imag();
  double result = floor(abs(complex<double>(delta_x, delta_y))*10000)/10000;
   return pow(result,2.0);
   //return abs(complex<double>(delta_x, delta_y));
}
