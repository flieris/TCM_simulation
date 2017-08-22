#ifndef coder_h
#define coder_h
class Coder {
public:
   Coder();
   ~Coder();

   unsigned int encode(unsigned int data);

   unsigned int getState();
private:
   unsigned int state;
};

#endif // !coder_h




