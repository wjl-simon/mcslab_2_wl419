#include <iostream>
#include "errors.h"
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"
#include "helper.h"


using namespace std;

// The enigma machine
char EnigmaMachine(char &ch, Plugboard& pb, Rotor& rt1, Rotor& rt2, Rotor& rt3, Reflector& rf);

int main(int argc, char**argv)
{

  Plugboard pb(argv[1]); Reflector rf(argv[2]);
  Rotor rt1(argv[3],argv[argc-1]), rt2(argv[4],argv[argc-1]), rt3(argv[5],argv[argc-1]);

  cout << "Please input 20 uppercase letters" << endl;
  // Text from std input strea
  char original[20];
  char temp;
  //  cin.getline(original,15) >> ws;
  for(int i = 0; i < 20; i++)
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
  for(int i = 0; i < 20; i++)
    cout << original[i];
  cout << endl;

  // Encription
  cout << "the ciphertext is:" << endl;
  for(int i = 0; i < 20; i++)
    if(original[i]!='\0')
      cout << EnigmaMachine(original[i],pb,rt1,rt2,rt3,rf);
  cout << endl << endl;

  // Reset the three rotors and use them as the enigma machine at the receiver side
  rt1.SetPosToStartingPos(); rt2.SetPosToStartingPos(); rt3.SetPosToStartingPos();
  
  // Decription
  cout << "the decripted text is: " << endl;
  for(int i = 0; i < 20; i++)
    if(original[i]!='\0')
      cout << EnigmaMachine(original[i],pb,rt1,rt2,rt3,rf);
  cout << endl;
  
  return 0;
}



char EnigmaMachine(char& ch, Plugboard& pb, Rotor& rt1, Rotor& rt2, Rotor& rt3, Reflector& rf)
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
  // Rotors mapping backward
  rt3.MapBackwards(ch); rt2.MapBackwards(ch); rt1.MapBackwards(ch);
  // Plugboard again
  pb.SwapLetters(ch);
  return ch;
}
