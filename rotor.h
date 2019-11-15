#ifndef ROTOR_H
#define ROTOR_H

class Rotor
{
  static int currentRotorNum; // the current number of all Rotor instances
  int rotorLabel; // the lable of an instance i.e. this is the i-th instance
  bool isConfigLoaded; // flag for if the rotor cofiguration is loaded
  bool isStartingPosLoaded; // flag for if the rotor starting position is loaded
  int curRtMap[26]; // current rotor mapping e.g.. the mapping for the i-th letter = curRtMap[i] + i
  int notch[26]; // an array of the notches
  int startingPos; // the starting position of this rotor
  int rotorTop; // the position at the top of the rotor
  
  /* Return false if the file connects a contact with itself or with more than one other  */
  bool IsLegalContact(int mapping[], int notch[]);

  
 public:
  /* Constructors*/
  Rotor(const char* rtConfigFilename, const char* rtStartingPosilename);

  /* Load the configuration for this rotor  */
  int LoadConfig(const char* rtConfigFilename);

  /* Load the Starting position of the rotors */
  int LoadStartingPos(const char* rtStartPosFilename);

  /*Functionality 2: Rotate a rotor */
  void Rotate();

  /* Set the Rotor's position to the designated starting position */
  void SetPosToStartingPos();
  
  /* Check if there is a notch at the top absolute reference posiion*/
  bool IsNotchAtTop();
  
  /* Rotate due to the rotor to its right whose absolute position hit a notch 
     Flag is the returned value of the DoMapping() member of the rotor to its right */
  void RotateDueToNotch(bool flag);

  /* Functionality 1.1: Map the letters forwards */
  void MapForwards(char& ch);

  /* Functionality 1.2: Map the letters backwards */
  void MapBackwards(char& ch);

  /* Destructor */
  ~Rotor();
};



#endif
