#ifndef REFLECTOR_H
#define REFLECTOR_H

#include "errors.h"
#include <fstream>
#include <iostream>

class Reflector
{
  char letters[26]; // 13 pairs of mappings of letters i.e. (let[0],let[1]),(let[2],let[3]),ect
  bool isLoaded; // flag for if sucessfully loaded
    
  /* Return true if given a white sapce, otherwise false */
  bool IsWhiteSpace(char ch);
  /* Return true if given a digit(0-9), otherwise false  */
  bool IsDigit(char ch);
  /* Return false if the file connects a contact with itself or with more than one other  */
  bool IsLegalContact();
  /* Convert 0-based letters into char */
  char Letter0Based2Char(int num); // convert from 1-digit num
  char Letter0Based2Char(char tens, char ones); // convert from 2-digit num
  
 public:
  /* Default constructor */
  Reflector();
  /* Load the reflector configuration */
  int LoadConfig(const char* rfConfigFileName);
  /* Swap the letters (the functionality of reflector) */
  void SwapLetters(char& letter); 
};


#endif
