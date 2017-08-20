#include <iostream>
#include <fstream>
#include "generator.h"
#include "transmitter.h"


using namespace std;

int main() {

   Generator *source = new Generator(123);
   Transmitter *t = new Transmitter();
   unsigned int symbol, encoded_symbol;
   fstream test_file("test.txt", std::ios::out | std::ios::binary | std::ios::trunc);
   cout << "Conducting test" << endl;
   cout << (2 & 1) << endl;
   for (int i = 0; i < 10; ++i) {
      symbol = static_cast<int>(source->Random(0,4));
      cout << "Symbol = " << symbol << endl;
      encoded_symbol = t->encode(symbol);
      cout << "Encoded symbol = "<< encoded_symbol << endl;
   }
   cout << symbol;
   test_file.close();
   system("pause");
}