#ifndef path_h
#define path_h

struct Path {
//public:

   int previous_ = 0;
   int next_ = 0;
   double metric_ = 0.0;
   int input_ = 0;
};
#endif // !path_h
