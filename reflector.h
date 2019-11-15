#ifndef REFLECTOR_H
#define REFLECTOR_H

class Reflector
{
  char letters[26]; // 13 pairs of mappings of letters i.e. (let[0],let[1]),(let[2],let[3]),ect
  bool isLoaded; // flag for if sucessfully loaded
    
  /* Return false if the file connects a contact with itself or with more than one other,
     true otherwise
  */
  bool IsLegalContact(char mapping[]);

  
 public:
  /* Constructors */
  Reflector(const char* rfConfigFileName);
  
  /* Load the reflector configuration */
  int LoadConfig(const char* rfConfigFileName);
  
  /* Swap the letters (the functionality of reflector) */
  void SwapLetters(char& letter); 
};


#endif
