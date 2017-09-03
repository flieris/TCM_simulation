#ifndef path_h
#define path_h

struct Path {
public:
   friend bool operator>(const Path& left,const  Path& right) {
      return left.path_metric_ > right.path_metric_;
   }
   int previous_state_;
   int next_state_;
   double path_metric_;
   int input_state_;
};
#endif // !path_h
