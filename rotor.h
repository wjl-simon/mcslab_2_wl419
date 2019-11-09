#ifndef ROTOR_H
#define ROTOR_H

class Rotor
{
  static int currentRotorNum; // the current number of all Rotor instances
  int rotorLabel; // the lable of an instance i.e. this is the i-th instance
  bool isConfigLoaded;
  bool isStartingPosLoaded;
  int mapAbs2Abs[26]; // mapping between absolue positions e.g. from 12 o'clock to 5 o'clock
  int letterAtAbsPos[26]; // letterOnAbsPos[i] is the 0-based letter at abs position i
  int notch[26]; // an array of letters who has notches
  int startingPos; // starting position of this rotor

  
  /* Return false if the file connects a contact with itself or with more than one other  */
  bool IsLegalContact(int mapping[], int notch[]);
  /*Functionality 2: Rotate the rotor */
  void Rotate();
  
 public:
  /* Constructors*/
  Rotor();
  Rotor(const char* rtConfigFilename, const char* rtStartingPosilename);
  /* Load the configuration for this rotor  */
  int LoadConfig(const char* rtConfigFilename);
  /* Load the Starting position of the rotors */
  int LoadStartingPos(const char* rtStartPosFilename);
  /* Set the Rotor's position to the designated starting position */
  void SetPosToStartingPos();
  /* Functionality 1.1: Map the letters forwards */
  bool MapForwards(char& ch);
  /* Functionality 1.2: Map the letters backwards */
  void MapBackwards(char& ch);
  /* Rotate due to the rotor to its right whose absolute position hit a notch 
     Flag is the returned value of the DoMapping() member of the rotor to its right */
  void RotateDueToNotch(bool flag);
  /* Destructor */
  ~Rotor();
};



#endif
