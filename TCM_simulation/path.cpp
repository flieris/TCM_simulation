#include "path.h"

Path::Path() {
   previous_state_ = 0;
   next_state_ = 0;
   path_metric_ = 0.0;
   input_state_ = 0;
}

Path::Path(int prv_state, int nxt_state, int input) {
   previous_state_ = prv_state;
   next_state_ = nxt_state;
   input_state_ = input;
   path_metric_ = 0.0;
}

Path::~Path() {

}

int Path::getPreviousState()
{
   return previous_state_;
}

void Path::setPreviousState(int state)
{
   previous_state_ = state;
}

int Path::getNextState()
{
   return next_state_;
}

void Path::setNextState(int state)
{
   next_state_ = state;
}

double Path::getPathMetric()
{
   return path_metric_;
}

void Path::setPathMetric(double metric)
{
   path_metric_ = metric;
}

int Path::getInputState()
{
   return input_state_;
}

void Path::setInputState(int input)
{
   input_state_ = input;
}
