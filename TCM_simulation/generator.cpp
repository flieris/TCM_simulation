#include "generator.h"
#include <cmath>

Generator::Generator(int kernel) {
   kernel_ = kernel;
   m_ = 2147483647.0;
   a_ = 16807;
   q_ = 127773;
   r_ = 2836;
}

Generator::~Generator() {

}

double Generator::Random() {
   int h = kernel_ / q_;
   kernel_ = a_*(kernel_ - q_*h) - r_*h;
   if (kernel_ < 0) kernel_ = kernel_ + static_cast<int>(m_);
   return kernel_ / m_;
}

double Generator::Random(int min, int max) {
   return Random()*(int)(max - min) + min;
}

double Generator::RandomVar(double min, double max) {
   return Random()*(double)(max - min) + min;
}
int Generator::getKernel() {
   return kernel_;
}

void Generator::setKernel(int value) {
   kernel_ = value;
}