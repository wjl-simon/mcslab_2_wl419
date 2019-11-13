#include <iostream>
#include "errors.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include "helper.h"
#include <string>


using namespace std;

// The enigma machine
char EnigmaMachine(char &ch, Plugboard& pb, Rotor* rt[], int rtNum, Reflector& rf);

int main(int argc, char**argv)
{

  // Text to be encripted
  string text; getline(cin,text); int const TEXTLENG = text.length();
  //cout << "the original one:" << endl << text << endl;
  
  // Enigma setting up
  int const ROTORNUM = argc - 4;
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
      if(text[i]>='A' && text[i]<='Z')
        cout << EnigmaMachine(text[i],pb,nullptr,ROTORNUM,rf);
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
    Rotor* rt[ROTORNUM];
    for(int i = 0; i < ROTORNUM; i++)
      rt[i] = new Rotor(argv[i+3],argv[argc-1]); // argv[argc-1] is the starting position file;


    // Encription
    //cout << "encription:"<<endl;
    for(int i = 0; i < TEXTLENG; i++)
      if(text[i]>='A' && text[i]<='Z')
        cout << EnigmaMachine(text[i],pb,rt,ROTORNUM,rf);
      else if(IsWhiteSpace(text[i]))
        continue;
      else
      {
        cerr << "INVALID INPUT CHARACTER";
        return INVALID_INPUT_CHARACTER;
      }

    /*    
    // decription
    cout << endl << "decription:"<<endl;
    for(int i = 0; i < ROTORNUM; i++)
      rt[i]->SetPosToStartingPos();
        
    for(int i = 0; i < TEXTLENG; i++)
      if(text[i]>='A' && text[i]<='Z')
        cout << EnigmaMachine(text[i],pb,rt,ROTORNUM,rf);
      else if(IsWhiteSpace(text[i]))
        continue;
      else
      {
        cerr << "INVALID INPUT CHARACTER";
        return INVALID_INPUT_CHARACTER;
      }
    */
    
    // delete the rts
    for(int i = 0; i < ROTORNUM; i++)
      delete rt[i];
  }
  
  return 0;
}




char EnigmaMachine(char& ch, Plugboard& pb, Rotor* rt[], int rtNum, Reflector& rf)
{
  

  // Rotors mapping forward

  /*
  bool flag = rt[0]->MapForwards(ch);
  rt[1]->RotateDueToNotch(flag); // rotate rt2 if a notch is at zero abs ref pos in rt1
  flag = rt[1]->MapForwards(ch);
  rt[2]->RotateDueToNotch(flag); // rotate rt3 if a notch is at zero abs ref pos in rt3
  rt[2]->MapForwards(ch);
  */  
  if(rt)
  {
    // When a key is pressed a rotation happens at the rightmost rotor before closing the circuit
    rt[0]->Rotate();

    // Plugboard Swapping
    pb.SwapLetters(ch);

    // Rotor Mapping
    bool flag = false;
    for(int i = 0; i < rtNum; i++)
      {
        flag = rt[i]->MapForwards(ch);
        if(i != rtNum-1) // the (rtNum-1)-th rotor won't have the next one rotated 
          rt[i+1]->RotateDueToNotch(flag);
      }

    // Reflector
    rf.SwapLetters(ch);

    // Rotor maping backwards
    for(int i = rtNum-1; i >= 0; i--) 
      rt[i]->MapBackwards(ch);

    // Plugboard again
    pb.SwapLetters(ch);
    
  }
  else
  {
    // Plugboard Swapping
    pb.SwapLetters(ch);
    // Reflector
    rf.SwapLetters(ch);
    // Plugboard again
    pb.SwapLetters(ch);
    
  }
  
  return ch;
}
