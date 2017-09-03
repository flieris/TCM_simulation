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
   next_state_.push_back({ 0,2,0,2 });
   next_state_.push_back({ 0,2,0,2 });
   next_state_.push_back({ 1,3,1,3 });
   next_state_.push_back({ 1,3,1,3 });
   output_state_.push_back({ 0,2,4,6 });
   output_state_.push_back({ 2,0,6,4 });
   output_state_.push_back({ 1,3,5,7 }); 
   output_state_.push_back({ 3,1,7,5 });
   trellis_memory_ = new std::vector<std::vector<Path*>*>{};
   //minimal_paths_ = new std::vector<Path*>();
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
   for (int previous_state = 0; previous_state < 4; previous_state++) {
      for (int next_state = 0; next_state < 4; next_state++) {
         // nowa scie¿ka z aktualnego stanu do nastêpnego stanu
         auto next = next_state_[previous_state][next_state];
         Path *path_ = new Path();
         path_->input_state_ = next_state;
         path_->previous_state_ = previous_state;
         path_->next_state_ = next;
         int output = output_state_[previous_state][next];
         std::cout << "Output state: " << output << std::endl;
         // lokalna metryka sciezki (tylko pomiedzy dwoma punktami)
         auto distance = getDistance(input_data, output);
         std::cout << "distance between 2 points: " << input_data << " and " << output << " is " << getDistance(input_data, output) << std::endl;
         //dot¹d jest ok
         
         if (trellis_memory_->size() > 0) {
            //uwzglêdnienie metryki globalnej wraz z metryka lokalna
            path_->path_metric_ = distance + trellis_memory_->back()->operator[](previous_state)->path_metric_;
         }
         else {
            path_->path_metric_ = distance;
         }
         if ((*minimal_paths_)[next] == nullptr) {
            (*minimal_paths_)[next] = path_;
         }
         else if ((*minimal_paths_)[next]->path_metric_ > path_->path_metric_) {
               (*minimal_paths_)[next] = path_;
            }
            
         std::cout << "path metric: " << (*minimal_paths_)[next]->path_metric_ << std::endl;
      }
      
   }
   //aktualizacja pamieci metryk
   for (int i = 0; i < 4; i++) {
      std::cout << "path metric: " << (*minimal_paths_)[i]->path_metric_ << std::endl;
   }
   trellis_memory_->push_back(minimal_paths_);
   int read_index = 0, write_index = 0;
   double metric_sums_[4] = { 0.0,0.0,0.0,0.0 };
   std::cout << "memeory size: " << trellis_memory_->size() << std::endl;
   for (int i = 0; i < trellis_memory_->size(); i++) {
      std::cout << "memory path size: " << trellis_memory_->operator[](i)->size() << std::endl;
      for (int j = 0; j < trellis_memory_->operator[](i)->size(); j++) {
         metric_sums_[j] += trellis_memory_->operator[](i)->operator[](j)->path_metric_;
      }
   }
   int mininal_index = 0;
   for (int i = 0; i < 4; i++) {
      if (metric_sums_[i] < metric_sums_[mininal_index]) {
         mininal_index = i;
      }
      std::cout << "Metric sums in: " << i << ' ' << metric_sums_[i] << std::endl;
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
   return pow(abs(std::complex<double>(delta_x, delta_y)),2.0);
}