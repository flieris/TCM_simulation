#include "decoder.h"
#include <complex>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

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

   
   next_state_.push_back({ 0,1,0,1 });
   next_state_.push_back({ 2,3,2,3 });
   next_state_.push_back({ 1,0,1,0 });
   next_state_.push_back({ 2,3,2,3 });
   output_state_.push_back({ 0,2,4,6 });
   output_state_.push_back({ 1,3,5,7 });
   output_state_.push_back({ 0,2,4,6 }); 
   output_state_.push_back({ 3,1,7,5 });
  
   /*
   next_state_.push_back({ 0,2,0,2 });
   next_state_.push_back({ 2,0,2,0 });
   next_state_.push_back({ 1,3,1,3 });
   next_state_.push_back({ 3,1,3,1 });
   output_state_.push_back({ 0,2,4,6 });
   output_state_.push_back({ 0,2,4,6 });
   output_state_.push_back({ 1,3,5,7 });
   output_state_.push_back({ 3,1,7,5 });
   */
   trellis_memory_ = new std::vector<std::vector<Path*>*>{};


   
}

Decoder::~Decoder() {
   delete _8psk_constelation;
}

void Decoder::decode(std::complex<double> input_data) {
   //podreczna tablica sciezek minimalnych dla jednego odstepu modulacji
   auto minimal_paths_ = new std::vector<Path*>();
   minimal_paths_->clear();
   for (auto i = 0; i < 4; i++) {
      minimal_paths_->push_back(nullptr);
   }
   //calculate Euclidian distances between input signal and every point on constellation
   auto distances = new std::vector<double>;
   for (int i = 0; i < 8; i++) {
      distances->push_back(getDistance(input_data, i));
   }

   for (int from_state = 0; from_state < 4; from_state++) {
      for (int in_state = 0; in_state < 4; in_state++) {
         Path* new_path_ = new Path();
         new_path_->input_ = in_state;
         new_path_->previous__ = from_state;
         int next_state = next_state_[from_state][in_state];
         new_path_->next_ = next_state;
         auto out_state_ = output_state_[from_state][in_state];
         new_path_->metric_ = (*distances)[out_state_];
         std::cout << "Output state: " << out_state_ << " Input: " << new_path_->input_ << " Previous state: " << new_path_->previous__ << " Next state: " 
                  << new_path_->next_ << " Local path metric: "<< new_path_->metric_ <<std::endl;
         if (trellis_memory_->size() > 0) {
            new_path_->metric_ += trellis_memory_->back()->operator[](in_state)->metric_;
            std::cout << "new metric: " << new_path_->metric_ << " from_state: " << from_state <<std::endl;
         }
         if ((*minimal_paths_)[next_state] == nullptr) {
            (*minimal_paths_)[next_state] = new_path_;
         }
         else
            if ((*minimal_paths_)[next_state]->metric_ > new_path_->metric_) {
               (*minimal_paths_)[next_state] = new_path_;
            }
      }
   }
   trellis_memory_->push_back(minimal_paths_);
   auto sums_ = new std::vector<double>;
   for (auto i = 0; i < 4; i++) {
      sums_->push_back(0.0);
   }
   for (int i = 0; i < trellis_memory_->size(); i++) {
      for (int j = 0; j < trellis_memory_->operator[](i)->size(); j++) {
         std::cout << "Metrics in memeory: " << trellis_memory_->operator[](i)->operator[](j)->metric_ << " Previous state: "<< trellis_memory_->operator[](i)->operator[](j)->previous__
            << " Next state: " << trellis_memory_->operator[](i)->operator[](j)->next_ << " Input state: " << trellis_memory_->operator[](i)->operator[](j)->input_ <<std::endl;
         sums_->operator[](j) += trellis_memory_->operator[](i)->operator[](j)->metric_;
      }  
   }
   for (auto i = 0; i < 4; i++) {
      std::cout << "Sum of metrics for path: " << i << " : " << sums_->operator[](i) << std::endl;
   }
}

int Decoder::getDecodedData() {

   return decoded_data;
}

void Decoder::initMemory()
{
  
}

double Decoder::getDistance(std::complex<double> data, int constelation_point){
   double delta_x = data.real() - _8psk_constelation->operator[](constelation_point).real();
   double delta_y = data.imag() - _8psk_constelation->operator[](constelation_point).imag();
   //double result = floorf(abs(std::complex<double>(delta_x, delta_y))*100)/100;
   //std::cout << "distance: " << result << std::endl;
  // return pow(result,2.0);
   return abs(std::complex<double>(delta_x, delta_y));
}