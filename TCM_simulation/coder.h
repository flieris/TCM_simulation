#ifndef coder_h
#define coder_h
/*
   Coder class that implements coder 2/3, coder scheme implemented like in Ungerboeck article
   a1 -------------------------------------------- c1
   a2 -----------[state0]------[state1]-----
            |              |               |
            |              |               +------ c2
            |              |               |  
            ---------------|---------------
                           ----------------------- c3
*/
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




