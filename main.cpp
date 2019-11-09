#include <iostream>
#include "errors.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include "helper.h"


using namespace std;

void EnigmaMachine(char &ch, Plugboard& pb, Rotor& rt1, Rotor& rt2, Rotor& rt3, Reflector& rf);

int main(int argc, char**argv)
//int main()
{
  // char const* pbConfig = "plugboards/I.pb";
  //   char const* rfConfig = "reflectors/I.rf";
  //  char const* rt1Config = "rotors/I.rot";
  //  char const* rt2Config = "rotors/II.rot";
  //  char const* rt3Config = "rotors/III.rot";
  //  char const* rtStartingPos = "rotors/I.pos";

  // cout << "Please enter the path for: \n the executable, the pluboard configuration file, "
  //      << "the reflector configuration file, three of the rotor configuration files "
  //      << "and the three rotor's starting position file"
  //      << endl << endl;
  
  // Plugboard, rotors and reflector instance
  // Plugboard pb(argv[1]); Reflector rf(argv[2]);
  // Rotor rt1(argv[3],argv[argc-1]), rt2(argv[4],argv[argc-1]), rt3(argv[5],argv[argc-1]);

   Plugboard pb(argv[1]); Reflector rf(argv[2]);
  Rotor rt1(argv[3],argv[argc-1]), rt2(argv[4],argv[argc-1]), rt3(argv[5],argv[argc-1]);

   // Plugboard pb(pbConfig); Reflector rf(rfConfig);
   // Rotor rt1(rt1Config,rtStartingPos), rt2(rt2Config,rtStartingPos), rt3(rt3Config,rtStartingPos);

  //  Plugboard pb; Rotor rt1, rt2, rt3; Reflector rf;
  // // Loading configs
  // int errCode = 0;
  // errCode = pb.LoadConfig(pbConfig); if (errCode) return errCode;
  // errCode = rt1.LoadConfig(rt1Config); if (errCode) return errCode;
  // errCode = rt2.LoadConfig(rt2Config); if (errCode) return errCode;
  // errCode = rt3.LoadConfig(rt3Config); if (errCode) return errCode;
  // errCode = rt1.LoadStartingPos(rtStartingPos); if (errCode) return errCode;
  // errCode = rt2.LoadStartingPos(rtStartingPos); if (errCode) return errCode;
  // errCode = rt3.LoadStartingPos(rtStartingPos); if (errCode) return errCode;
  // rt1.SetPosToStartingPos(); rt2.SetPosToStartingPos(); rt3.SetPosToStartingPos();
  // errCode = rf.LoadConfig(rfConfig); if (errCode) return errCode;

  
  // cout << errCode << endl;
  /*
  char ch = 'Z';

  // Plugboard Swapping
  pb.SwapLetters(ch);
  cout << "After pb: " << ch << endl;
  
  // Rotors mapping forward
  bool flag1 = rt1.MapForwards(ch);
  cout << "after roto1: " <<ch << endl;
  rt2.RotateDueToNotch(flag1); // rotate rt2 if a notch is at zero abs ref pos in rt1
  bool flag2 = rt2.MapForwards(ch);
  cout << "after rotor2: " <<ch << endl;
  rt3.RotateDueToNotch(flag2); // rotate rt3 if a notch is at zero abs ref pos in rt3
  rt3.MapForwards(ch);
  cout << "after rotor3: " <<ch << endl;

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
  
  */

  
  // input
   char original[5] = {'A', 'Z', 'W', 'O', 'F'};
  cout << "the original text is:" << endl;
    for(int i = 0; i < 5; i++)
     if(original[i]!='\0')
       {
         cout << original[i];
       }
    cout << endl;
  //char output[15];
  //cin.getline(original,15) >> ws;
  //cout << endl << endl;
     cout << "the ciphertext is:" << endl;
   for(int i = 0; i < 5; i++)
     if(original[i]!='\0')
       {
         EnigmaMachine(original[i],pb,rt1,rt2,rt3,rf);
       cout << original[i];
       }
   cout << endl;
   cout << endl << endl << "the decrpted text is: " << endl;

   rt1.SetPosToStartingPos(); rt2.SetPosToStartingPos(); rt3.SetPosToStartingPos();
   
   for(int i = 0; i < 5; i++)
     if(original[i]!='\0')
       {
         EnigmaMachine(original[i],pb,rt1,rt2,rt3,rf);
         cout << original[i];
       }
  
  return 0;
  
}



void EnigmaMachine(char &ch, Plugboard& pb, Rotor& rt1, Rotor& rt2, Rotor& rt3, Reflector& rf)
{
  // Plugboard Swapping
  pb.SwapLetters(ch);
  //  cout << "After pb: " << ch << endl;
  
  // Rotors mapping forward
  bool flag1 = rt1.MapForwards(ch);
  // cout << "after roto1: " <<ch << endl;
  rt2.RotateDueToNotch(flag1); // rotate rt2 if a notch is at zero abs ref pos in rt1
  bool flag2 = rt2.MapForwards(ch);
  //cout << "after rotor2: " << ch << endl;
  rt3.RotateDueToNotch(flag2); // rotate rt3 if a notch is at zero abs ref pos in rt3
  rt3.MapForwards(ch);
  //cout << "after rotor3: " <<ch << endl;

  // reflector
  rf.SwapLetters(ch);
  //cout << "after reflector: " << ch << endl;

  // rotors mapping backward
  rt3.MapBackwards(ch);
  rt2.MapBackwards(ch);
  rt1.MapBackwards(ch);

  // plugboard again
  pb.SwapLetters(ch);
  //cout << "After pb: " << ch << endl;
  //return ch;
}
