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
   decoded_data = 0;
   
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
   trellis_index_ = 1;
   // trellis memory: matrix with 4 rows and 45 columns
   for (int i = 0; i < k_NUMBER_OF_STATES; i++) {
      for (int j = 0; j < k_TRELLIS_DEPTH; j++) {
         trellis_memory_[i][j] = nullptr;

      }
   }
}

Decoder::~Decoder() {
   delete _8psk_constelation;
}

void Decoder::decode(std::complex<double> input_data) {
   //calculate Euclidian distances between input signal and every point on constellation
   auto distances = new std::vector<double>;
   for (int i = 0; i < 8; i++) {
      distances->push_back(getDistance(input_data, i));
   }
   // testing right now -- local path array that saves EVERY path for further computation
   Path *hand_[4][4];
   for (int i = 0; i < k_NUMBER_OF_STATES; i++) {
      for (int j = 0; j < k_NUMBER_OF_STATES; j++) {
         hand_[i][j] = nullptr;
      }
   }
   for (int from_state = 0; from_state < k_NUMBER_OF_STATES; ++from_state) {
      for (int in_state = 0; in_state < k_NUMBER_OF_STATES; ++in_state) {
         Path* new_path_ = new Path();
         new_path_->input_ = in_state;
         new_path_->previous_ = from_state;
         int next_state = next_state_[from_state][in_state];
         new_path_->next_ = next_state;
         auto out_state_ = output_state_[from_state][in_state];
         new_path_->metric_ = (*distances)[out_state_];
         hand_[from_state][in_state] = new_path_;
         //std::cout << "Output state: " << out_state_ << " Input: " << new_path_->input_ << " Previous state: " << new_path_->previous__ << " Next state: " 
                  //<< new_path_->next_ << " Local path metric: "<< new_path_->metric_ <<std::endl;
      }
   }
   for (int from_state = 0; from_state < k_NUMBER_OF_STATES; ++from_state) {
      for (int in_state = 0; in_state < k_NUMBER_OF_STATES; ++in_state) {
         std::cout << "Input: " << hand_[from_state][in_state]->input_ << " Previous state: " << hand_[from_state][in_state]->previous_
            << " Next state: " << hand_[from_state][in_state]->next_ << " Local metric: " << hand_[from_state][in_state]->metric_ << std::endl;
      }
   }
   //std::cout << trellis_memory_->size() << std::endl;
   // now we should look which path is the best (the cheapest) 
   for (int i = 0; i < k_NUMBER_OF_STATES; i++) {
      // each previous state has 4 possible paths to 2 different states + trellis_memory_->operator[](i)->operator[](read_index_)->metric_
      for (int j = 0; j < k_NUMBER_OF_STATES; j++) {
         if (trellis_memory_[i][trellis_index_-1] != nullptr) {
            hand_[i][j]->metric_ += trellis_memory_[i][trellis_index_-1]->metric_;
            if (trellis_memory_[i][trellis_index_] == nullptr) {
               trellis_memory_[i][trellis_index_] = hand_[i][j];
            }
            else {
               if (trellis_memory_[i][trellis_index_ - 1]->metric_ > trellis_memory_[i][trellis_index_]->metric_) {
                  trellis_memory_[i][trellis_index_] = hand_[i][j];
               }
               if (trellis_memory_[i][trellis_index_]->metric_ > hand_[i][j]->metric_) {
                  trellis_memory_[i][trellis_index_] = hand_[i][j];
               }
            }
         }
         else {
            trellis_memory_[i][trellis_index_] = hand_[i][j];
         } 
      }
      std::cout << "Best path from state: " << i << " is: " << trellis_memory_[i][trellis_index_]->metric_
         << " and leads to state: " << trellis_memory_[i][trellis_index_]->next_ << " and input: " << trellis_memory_[i][trellis_index_]->input_ 
         << " just for testing: " << trellis_memory_[i][trellis_index_]->previous_ << std::endl;
   }
   if (trellis_index_ < 44) {
      trellis_index_++;
   }
   else {
      trellis_index_ = 1;
      for (int i = 0; i < k_NUMBER_OF_STATES; i++) {
         trellis_memory_[i][0] = trellis_memory_[i][44];
      }

   }
   //trellis_memory_->push_back(minimal_paths_);
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