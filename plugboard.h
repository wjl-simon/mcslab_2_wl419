#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include <iostream>
#include "errors.h"
#include <fstream>

using namespace std;

class Plugboard
{
  /*===== Data Member=====*/
  int letters[26]; // the 0-base letters i.e. '0' is 'A', '1' is 'B'
  int letterNum; // the number of numbers in the "letters" arrays
  bool isLoaded; // flag indicating if the plugboard has sucessfully loaded
    
  /* Return true if given a white space, otherwise false  */
  bool IsWhiteSpace(char ch);
  /* Return true if given a digit(0-9), otherwise false  */
  bool IsDigit(char ch);
  /* Return false if the file connects a contact with itself or with more than one other  */
  bool IsLegalContact();

 public:
  /* Default constructor */
  Plugboard();
  /* Load the plugboard configuration */
  int LoadConfig(char* pbConfigFileName);
  /* Swap the letters (the functionality of plugboard) */
  void SwapLetters(char& letter1, char& letter2);
};



#endif
