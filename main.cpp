#include <iostream>
#include <fstream>
#include "errors.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"

using namespace std;

int main()
{
  // Plugboard pg;
  // const char* name = "plugboards/I.pb";
  // int code =pg.LoadConfig(name);
  // cout << code << endl;
  //  char ch = 'I';
  //  pg.SwapLetters(ch);
  //  cout << ch << endl;
  Rotor rt1;
  const char* name = "rotors/II.rot";
  int code =pg.LoadConfig(name);
  cout << code << endl;
   char ch = 'I';
   pg.SwapLetters(ch);
   cout << ch << endl;
   
  return 0;
  
}
