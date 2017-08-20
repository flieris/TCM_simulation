#ifndef generator_h
#define generator_h

class Generator {
public:
   Generator(int kernel);
   ~Generator();

   double Random();
   double Random(int min, int max);

   int getKernel();
   void setKernel(int value);
private:
   int kernel_;
   double m_;
   int a_, q_, r_;

};
#endif // !generator_h
