#ifndef ROTOR_H
#define ROTOR_H

class Rotor
{
  int rotorLabel; // the lable of an instance i.e. this is the i-th instance
  int currentMap[26]; // the current 1-to-1 mapping from one letter to another, 0-based
  int notchPos[26]; // The abosulute position where there is a notch
  static int  currentRotorNum; // the current number of all Rotor instances
  //static int const TOTAL_ROTOR_NUM = 3; // total rotor numbers
  //static int startPos[TOTAL_ROTOR_NUM]; // the statring position of a rotor
  int startingPos; // starting position of this rotor

  /* Return true if given a white sapce, otherwise false */
  bool IsWhiteSpace(char ch);
  /* Return true if given a digit(0-9), otherwise false  */
  bool IsDigit(char ch);
  /* Return false if the file connects a contact with itself or with more than one other  */
  bool IsLegalContact();
  /* Convert 0-based letters () into Latin letters (char) */
  char Letter0Based2char(int num);
  /* Convert digit (0-9) from char into int */
  int DigitChar2Int(char digit);
  /*Functionality 2: Rotate the rotor */
  void Rotate();
  
 public:
  /* Default Constructor*/
  Rotor();
  /* Load the configuration for this rotor  */
  int LoadConfig(const char* rtConfigFilename);
  /* Load the Starting position of the rotors */
  static int LoadStartingPos(const char* rtStartPosFilename);
  /* Set the Rotor's position to the designated starting position */
  void SetPosToStartingPos();
  /* Functionality 1: Map the letters */
  void DoMapping(char& ch);
  /* Destructor */
  ~Rotor();
};



#endif
