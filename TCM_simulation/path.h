#ifndef path_h
#define path_h

struct Path {
public:

   int previous_;
   int next_;
   double metric_;
   int input_;
};
#endif // !path_h
