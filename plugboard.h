#ifndef PLUGBOARD_H
#define PLUGBOARD_H


class Plugboard
{
  char letters[26]; // the upper case letters. It remains unchanged if loading fails
  int letterNum; // number of numbers in letters array. It won't be changed if loading fails
  bool isLoaded; // flag for if config sucessfully loaded

  /* Return false if the file connects a contact with itself or with more than one other,
     true otherwise
  */
  bool IsLegalContact(char mapping[]);

  
 public:
  /* Constructors */
  Plugboard();
  Plugboard(const char* pbConfigFileName);
  
  /* Load the plugboard configuration */
  int LoadConfig(const char* pbConfigFileName);
  
  /* Swap the letters (the functionality of plugboard) */
  void SwapLetters(char& letter);
};


#endif
