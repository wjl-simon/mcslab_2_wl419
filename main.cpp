#include <iostream>
#include "errors.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include "helper.h"


using namespace std;

char EnigmaMachine(char ch, Plugboard& pb, Rotor& rt1, Rotor& rt2, Rotor& rt3, Reflector& rf);

//int main(int argc, char**argv)
int main()
{
  char const* pbConfig = "plugboards/I.pb";
  //char const* pbConfig = "rotors/II.rot";
    char const* rfConfig = "reflectors/I.rf";
   char const* rt1Config = "rotors/I.rot";
   char const* rt2Config = "rotors/II.rot";
   char const* rt3Config = "rotors/III.rot";
   char const* rtStartingPos = "rotors/I.pos";

  
  // Plugboard, rotors and reflector
  Plugboard pb;
  Rotor rt1, rt2, rt3; Reflector rf;
 
  // Loading configs
  int errCode = 0;
  errCode = pb.LoadConfig(pbConfig); if (errCode) return errCode;
  errCode = rt1.LoadConfig(rt1Config); if (errCode) return errCode;
  errCode = rt2.LoadConfig(rt2Config); if (errCode) return errCode;
  errCode = rt3.LoadConfig(rt3Config); if (errCode) return errCode;
  errCode = rt1.LoadStartingPos(rtStartingPos); if (errCode) return errCode;
  errCode = rt2.LoadStartingPos(rtStartingPos); if (errCode) return errCode;
  errCode = rt3.LoadStartingPos(rtStartingPos); if (errCode) return errCode;
  rt1.SetPosToStartingPos(); rt2.SetPosToStartingPos(); rt3.SetPosToStartingPos();
  errCode = rf.LoadConfig(rfConfig); if (errCode) return errCode;

  cout << errCode << endl;
  char ch = 'Z';

  // Plugboard Swapping
  pb.SwapLetters(ch);
  cout << "After pb: " << ch << endl;
  
  // Rotors mapping forward
  bool flag1 = rt1.MapForwards(ch);
  //cout << "after roto1: " <<ch << endl;
  rt2.RotateDueToNotch(flag1); // rotate rt2 if a notch is at zero abs ref pos in rt1
  bool flag2 = rt2.MapForwards(ch);
  //cout << "after rotor2: " <<ch << endl;
  rt3.RotateDueToNotch(flag2); // rotate rt3 if a notch is at zero abs ref pos in rt3
  rt3.MapForwards(ch);
  //cout << "after rotor3: " <<ch << endl;

  // reflector
  rf.SwapLetters(ch);
  cout << "after reflector: " << ch << endl;

  // rotors mapping backward
  rt3.MapBackwards(ch);
  rt2.MapBackwards(ch);
  rt1.MapBackwards(ch);

  // plugboard again
  pb.SwapLetters(ch);
  cout << "After pb: " << ch << endl;
  
  // // input
  // char original[100];
  // cin.getline(original,100) >> ws;
  // cout << endl << endl;
  
   // for(int i = 0; i < 100; i++)
   //   if(original[i]!='\0')
  //cout << EnigmaMachine(ch,pb,rt1,rt2,rt3,rf);

  // cout << endl;
  
  return 0;
  
}



char EnigmaMachine(char ch, Plugboard& pb, Rotor& rt1, Rotor& rt2, Rotor& rt3, Reflector& rf)
{
  // Plugboard Swapping
  pb.SwapLetters(ch);

  // Rotors mapping forward
  bool flag1 = rt1.MapForwards(ch);
  rt2.RotateDueToNotch(flag1); // rotate rt2 if a notch is at zero abs ref pos in rt1
  bool flag2 = rt2.MapForwards(ch);
  rt3.RotateDueToNotch(flag2); // rotate rt3 if a notch is at zero abs ref pos in rt3
  rt3.MapForwards(ch);

  // Reflector
  rf.SwapLetters(ch);

  // Rotors mapping backwards
  rt3.MapBackwards(ch); rt2.MapBackwards(ch); rt1.MapBackwards(ch);

  // Back to Plugboard
  pb.SwapLetters(ch);
  
  return ch;
}
