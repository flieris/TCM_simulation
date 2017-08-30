#ifndef path_h
#define path_h

class Path {
public:
   Path();
   Path(int prv_state, int nxt_state, int input);
   ~Path();

   int getPreviousState();
   void setPreviousState(int state);

   int getNextState();
   void setNextState(int state);

   double getPathMetric();
   void setPathMetric(double metric);

   int getInputState();
   void setInputState(int input);
private:
   int previous_state_;
   int next_state_;
   double path_metric_;
   int input_state_;
};
#endif // !path_h
