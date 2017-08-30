#include "decoder.h"
#include <complex>
#include <vector>
#include <iostream>

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
   next_state_.push_back({ 1,3,1,3 });
   next_state_.push_back({ 0,2,0,2 });
   next_state_.push_back({ 1,3,1,3 });
   output_state_.push_back({ 0,2,4,6 });
   output_state_.push_back({ 1,3,5,7 });
   output_state_.push_back({ 2,0,6,4 });
   output_state_.push_back({ 3,1,7,5 });
   memory_ = new std::vector<std::vector<Path*>*>;
   minimal_paths_ = new std::vector<Path*>();
   for (auto i = 0; i < 4; i++) {
      minimal_paths_->push_back(nullptr);
   }
}

Decoder::~Decoder() {
   delete _8psk_constelation;
}

void Decoder::decode(std::complex<double> input_data) {
   // Patrzymy na przejscia ze wszystkich stanow 
   for (int checked_state_ = 0; checked_state_ < 4; checked_state_++) {
      // sprawdzamy na przejscia z danego checked_state_ przyklad: ze stanu 0 mamy przejscia do dwoch stanow 0 i 2 (po 2 przejscia do kazdego stanu)
      for (int input_state = 0; input_state < next_state_[checked_state_].size(); input_state++) {
         // nowa scie¿ka z aktualnego stanu do nastêpnego stanu
         Path *path_ = new Path(checked_state_, next_state_[checked_state_][input_state], input_state);
         int output = output_state_[checked_state_][input_state];
         // metryka sciezki jest odlegloscia euklidesowa pomiedzy dana wejsciowa a punktem na konstelacji 
         path_->setPathMetric(getDistance(input_data, (*_8psk_constelation)[output]));
         if (!memory_->empty()) {
            path_->setPathMetric(path_->getPathMetric() + (*memory_->back())[checked_state_]->getPathMetric());
         }
         if ((*minimal_paths_)[next_state_[checked_state_][input_state]] == nullptr) {
            (*minimal_paths_)[next_state_[checked_state_][input_state]] = path_;
         }
         else
            if ((*minimal_paths_)[next_state_[checked_state_][input_state]]->getPathMetric() > path_->getPathMetric()) {
               (*minimal_paths_)[next_state_[checked_state_][input_state]] = path_;
            }
      }
   }
   memory_->push_back(minimal_paths_);
}

int Decoder::getDecodedData() {

   return 0;
}

double Decoder::getDistance(std::complex<double> data, std::complex<double> constelation_point)
{
   double delta_x = data.real() - constelation_point.real();
   double delta_y = data.imag() - constelation_point.imag();
   return abs(std::complex<double>(delta_x, delta_y));
}