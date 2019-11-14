#include <iostream>
#include "errors.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include "helper.h"
#include <string>


using namespace std;


/* The enigma encripts one character one time only while the state of the machine is preserved
   rt[]: an array of newed rotors where rt[0] is the left most one (null if no rotors)
   rtNum: number of rotors
 */
void EnigmaMachine(char &ch, Plugboard& pb, Rotor* rt[], int rtNum, Reflector& rf);


int main(int argc, char**argv)
{

  // Text to be encripted
  string plainText; getline(cin,plainText); int const TEXTLENG = plainText.length();
  
  // Enigma setting up
  int const ROTORNUM = argc - 4; // rotor numbers
  
  if(ROTORNUM < 0)
  {
    cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions" << endl;
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }
  else if(argc == 0)
  {
    // The plubboard and the relector
    Plugboard pb(argv[1]); Reflector rf(argv[2]);

    // Encription
    for(int i = 0; i < TEXTLENG; i++)
    {
      if(plainText[i]>='A' && plainText[i]<='Z')
        EnigmaMachine(plainText[i],pb,nullptr,ROTORNUM,rf);
      else if(IsWhiteSpace(plainText[i]))
        continue;
      else
      {
        cerr << plainText[i] << " is not a valid input character (input characters must"
             <<" be upper case letters A-Z)!" << endl;
        return INVALID_INPUT_CHARACTER;
      }
    }

    // Print the cipher text
    cout << plainText << endl;
  }
  else // have one or more rotors
  {
    // The plubboard and the relector
    Plugboard pb(argv[1]); Reflector rf(argv[2]);

    // Rotors
    // !!! NB: rt[0] is the leftmost rotor while the signal firstly enters the rightmost rotor
    Rotor* rt[ROTORNUM];
    for(int i = 0; i < ROTORNUM; i++)
       rt[i] = new Rotor(argv[i+3],argv[argc-1]); // argv[argc-1] is the starting position file;

    // Encription
    for(int i = 0; i < TEXTLENG; i++)
    {
      if(plainText[i]>='A' && plainText[i]<='Z')
        EnigmaMachine(plainText[i],pb,rt,ROTORNUM,rf);
      else if(IsWhiteSpace(plainText[i]))
        continue;
      else
      {
        cerr << plainText[i] << " is not a valid input character (input characters must"
             <<" be upper case letters A-Z)!" << endl;
        return INVALID_INPUT_CHARACTER;
      }
    }

    // Print the cipher text
    cout << plainText << endl;

    // delete the rts
    for(int i = 0; i < ROTORNUM; i++)
      delete rt[i];
  }
  
  return 0;
}

/* The enigma encripts one character one time only while the state of the machine is preserved
   rt[]: an array of newed rotors where rt[0] is the left most one (null if no rotors)
   rtNum: number of rotors
 */
void EnigmaMachine(char& ch, Plugboard& pb, Rotor* rt[], int rtNum, Reflector& rf)
{
  if(rt) // has rotors
  {
    // When a key is pressed a rotation happens at the rightmost rotor before closing the circuit
    rt[rtNum-1]->Rotate();

    // Rotor engagement
    bool flag = false;
    for(int i = rtNum-1; i >= 0; i--)
    {
      flag = rt[i]->IsNotchAtTop();
      if(!flag) break; // if one rotor's notch isn't at the top, all rotors to its left won't rotate
      
      if(i != 0) rt[i-1]->RotateDueToNotch(flag);
    }

    //===Circuit Close now!===
    //Plugboard Swapping
    pb.SwapLetters(ch);

    // Rotor Mapping forwards
    for(int i = rtNum-1; i >= 0; i--) rt[i]->MapForwards(ch);

    // Reflector
    rf.SwapLetters(ch);

    // Rotor maping backwards
    for(int i = 0; i < rtNum; i++)  rt[i]->MapBackwards(ch);

    // Plugboard again
    pb.SwapLetters(ch);
  }
  else
  {
    // Plugboard Swapping, Reflector, Plugboard again 
    pb.SwapLetters(ch); rf.SwapLetters(ch); pb.SwapLetters(ch);
  }
}
