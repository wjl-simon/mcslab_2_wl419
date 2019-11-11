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

  // The plubboard, the relector and the rotors
  Plugboard pb(argv[1]); Reflector rf(argv[2]);

  if(argc < 4) return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  else if(argc == 4)
  {
    cout << "Please input 26 uppercase letters" << endl;
    // Text from std input strea
    char original[26];
    char temp;
    //  cin.getline(original,15) >> ws;
    for(int i = 0; i < 26; i++)
      {
        cin >> ws >> temp;
        if( temp >= 'A' && temp <='Z')
          original[i] = temp;
        else
          continue;
      }
    cout << endl << endl;

    // print out the text
    cout << "the original text is:" << endl;
    for(int i = 0; i < 26; i++)
      cout << original[i];
    cout << endl;

    // Encription
    cout << "the ciphertext is:" << endl;
    for(int i = 0; i < 26; i++)
      if(original[i]!='\0')
        cout << EnigmaMachine(original[i],pb,nullptr,argc-4,rf);
    cout << endl << endl;
    
  }
  else // have one or more rotors
  {
    // Rotors
    Rotor* rt[argc-4];
    for(int i = 0; i < argc-4; i++)
      rt[i] = new Rotor(argv[i+3],argv[argc-1]); // argv[argc-1] is the starting position file;


    cout << "Please input 26 uppercase letters" << endl;
    // Text from std input strea
    char original[26];
    char temp;
    //  cin.getline(original,15) >> ws;
    for(int i = 0; i < 26; i++)
      {
        cin >> ws >> temp;
        if( temp >= 'A' && temp <='Z')
          original[i] = temp;
        else
          continue;
      }
    cout << endl << endl;

    // print out the text
    cout << "the original text is:" << endl;
    for(int i = 0; i < 26; i++)
      cout << original[i];
    cout << endl;

    // Encription
    cout << "the ciphertext is:" << endl;
    for(int i = 0; i < 26; i++)
      if(original[i]!='\0')
        cout << EnigmaMachine(original[i],pb,rt,argc-4,rf);
    cout << endl << endl;

    /*
    // Reset the three rotors and use them as the enigma machine at the receiver side
    rt1.SetPosToStartingPos(); rt2.SetPosToStartingPos(); rt3.SetPosToStartingPos();
  
    // Decription
    cout << "the decripted text is: " << endl;
    for(int i = 0; i < 26; i++)
      if(original[i]!='\0')
        cout << EnigmaMachine(original[i],pb,rt1,rt2,rt3,rf);
    cout << endl;
    */

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
  if(!rt)
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
  if(!rt)
  {
    for(int i = 0; i < rtNum; i++) 
      rt[i]->MapBackwards(ch);
  }
  // Plugboard again
  pb.SwapLetters(ch);
  return ch;
}
