#include <iostream>
#include "errors.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include "helper.h"


using namespace std;

// The enigma machine
char EnigmaMachine(char &ch, Plugboard& pb, Rotor* rt[], int rtNum, Reflector& rf);

int main(int argc, char**argv)
{

  // Text to be encripted
  string text; getline(cin,text); int const TEXTLENG = text.length();
  
  // Enigma setting up
  if(argc < 4)
  {
    cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions" << endl;
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }
  else if(argc == 4)
  {
    // The plubboard and the relector
    Plugboard pb(argv[1]); Reflector rf(argv[2]);

    // Encription
    for(int i = 0; i < TEXTLENG; i++)
      if(text[i]>='A' && text[i]<='Z')
        cout << EnigmaMachine(text[i],pb,nullptr,argc-4,rf);
      else if(IsWhiteSpace(text[i]))
        continue;
      else
      {
        cerr << text[i]
             << " is not a valid input character (input characters must be upper case letters A-Z)!"
             << endl;
        return INVALID_INPUT_CHARACTER;
      }
  }
  else // have one or more rotors
  {
    // The plubboard and the relector
    Plugboard pb(argv[1]); Reflector rf(argv[2]);

    // Rotors
    Rotor* rt[argc-4];
    for(int i = 0; i < argc-4; i++)
      rt[i] = new Rotor(argv[i+3],argv[argc-1]); // argv[argc-1] is the starting position file;

    // Encription
    for(int i = 0; i < TEXTLENG; i++)
      if(text[i]>='A' && text[i]<='Z')
        cout << EnigmaMachine(text[i],pb,rt,argc-4,rf);
      else if(IsWhiteSpace(text[i]))
        continue;
      else
      {
        cerr << "INVALID INPUT CHARACTER";
        return INVALID_INPUT_CHARACTER;
      }
    
    // delete the rts
    for(int i = 0; i < argc-4; i++)
      delete rt[i];
  }
  
  return 0;
}




char EnigmaMachine(char& ch, Plugboard& pb, Rotor* rt[], int rtNum, Reflector& rf)
{
  // Plugboard Swapping
  pb.SwapLetters(ch);
  // Rotors mapping forward
  /*
  bool flag1 = rt1.MapForwards(ch);
  rt2.RotateDueToNotch(flag1); // rotate rt2 if a notch is at zero abs ref pos in rt1
  bool flag2 = rt2.MapForwards(ch);
  rt3.RotateDueToNotch(flag2); // rotate rt3 if a notch is at zero abs ref pos in rt3
  rt3.MapForwards(ch);
  */
  if(rt)
  {
    bool flag = false;
    for(int i = 0; i < rtNum; i++)
      {
        flag = rt[i]->MapForwards(ch);
        if(i != rtNum-1) // the (rtNum-1)-th rotor won't have the next one rotated 
          rt[i+1]->RotateDueToNotch(flag);
      }
  }
  // Reflector
  rf.SwapLetters(ch);
  // Rotors mapping backward
  if(rt)
  {
    for(int i = 0; i < rtNum; i++) 
      rt[i]->MapBackwards(ch);
  }
  // Plugboard again
  pb.SwapLetters(ch);
  return ch;
}
