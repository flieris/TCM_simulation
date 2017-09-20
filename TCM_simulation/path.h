#ifndef path_h
#define path_h

/* 
   Struct that hold information about one transition for previous_ state to next_ state,
   it also holds information about cost of transition (metric_) and input symbol that triggered the transition
*/
struct Path {

   int previous_ = 0;
   int next_ = 0;
   double metric_ = 0.0;
   int input_ = 0;
};
#endif // !path_h
