#ifndef PLUGBOARD_H
#define PLUGBOARD_H

//using namespace std;

class Plugboard
{
  //int letters[26]; // 0-base letters, e.g. '0' means 'A', '1' means 'B'
  char letters[26]; // the upper case letters. It remains unchanged if loading fails
  int letterNum; // number of numbers in letters array. It won't be changed if loading fails
  bool isLoaded; // flag for if sucessfully loaded
    
/* Return true if given a white sapce, otherwise false */
bool IsWhiteSpace(char ch);
/* Return true if given a digit(0-9), otherwise false  */
bool IsDigit(char ch);
/* Return false if the file connects a contact with itself or with more than one other  */
bool IsLegalContact();

 public:
/* Default constructor */
Plugboard();
/* Load the plugboard configuration */
int LoadConfig(const char* pbConfigFileName);
/* Swap the letters (the functionality of plugboard) */
void SwapLetters(char& letter);
};


#endif
