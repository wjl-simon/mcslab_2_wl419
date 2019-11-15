#include <iostream>
#include <fstream>
#include <cstdlib>
#include "errors.h"
#include "rotor.h"
#include "helper.h"

using namespace std;


int Rotor::currentRotorNum = 0;


/* Return false if the file connects a contact with itself or with more than one other  */
bool Rotor::IsLegalContact(int mapping[], int notch[])
{
  // It's legal means every digit in currentMap is unique
  for(int i = 0; i < 26; i++)
  {
    for(int j = i+1; j < 26; j++)
    {
      if(mapping[i] == mapping[j])
      {
        cerr << "Invalid mapping of input " << j << " to output " << mapping[i]
             << " (output " << mapping[i] << " is already mapped to from input "
             << i << ") in in rotor file rotor.rot "
             << endl;
        return false;
      }
    }
  }
  
  // Notch postions should also be unique
  for(int i = 0; i < 26 && notch[i]!=-1; i++)
  {
    for(int j = i+1; j < 26 && notch[j]!=-1; j++)
      if(notch[i] == notch[j]) return false;
  }
  
  return true;
}


/* Constructors*/
Rotor::Rotor(const char* rtConfigFilename, const char* rtStartingPosilename):
  isConfigLoaded(false),isStartingPosLoaded(false),startingPos(-1),rotorTop(0)
{
  for(int i = 0; i < 26; i++){ curRtMap[i] = -1; notch[i] = -1; }
  
  currentRotorNum++; // increment by 1 for each new rotor instance
  rotorLabel = currentRotorNum - 1; // the rotorLabel starts from 0

  // Load configs
  int e = LoadConfig(rtConfigFilename); if(e) exit(e);
  e = LoadStartingPos(rtStartingPosilename); if(e) exit(e);

  // Set starting position of the rotor
  SetPosToStartingPos();
}


/* Load the configuration for this rotor  */
int Rotor::LoadConfig(const char* rtConfigFilename)
{
  //=== 1. Open the file giving rotor configuration
  ifstream ipfile; ipfile.open(rtConfigFilename);
  if(ipfile.fail())
  {
    cerr << "Cannot open rotor configuration file" << endl;
    exit(ERROR_OPENING_CONFIGURATION_FILE);
  }

  //=== 2. Get the numbers (the letters) from the texture file
  char current, next; // current and next char read from the file
  ipfile >> ws; // filestream starts from first non-ws char

  int mapping_temp[26]; // content in the config file for the mapping
  int notch_temp[26]; // notches  
  for(int i = 0; i < 26; i++){ mapping_temp[i] = -1; notch_temp[i] = -1; }
  
  int i; // counter
  for(i = 0; i < 26*2 && !ipfile.eof(); i++)
  {
    ipfile.get(current); next = ipfile.peek();

    if(IsDigit(current))
    {
      if(IsDigit(next)) // current and next are digits: a two-digit number
      {
        // Test if this two-digit number is between 10 and 25
        if(current=='1' || (current=='2' && next>='0' && next<='5'))
        {
          if(i < 26) // the first 26 parameters are for the mapping
            mapping_temp[i] = 10 * DigitChar2Int(current) + DigitChar2Int(next); 
          else // the rest is for the notches
            notch_temp[i-26] = 10 * DigitChar2Int(current) + DigitChar2Int(next);
          
          ipfile.get(current); ipfile >> ws; // skip the "next" to get the one after
        }
        else
        {
          cerr << "INVALID INDEX (not between 0 and 25) in the rotor config!" << endl;
          return INVALID_INDEX;
        }
      }
      else if(IsWhiteSpace(next)) // current is a digit, next is a ws: a one-digit number
      {
        if(i < 26) // the first 26 parameters are for the mapping
          mapping_temp[i] = DigitChar2Int(current);
        else // the rest is for the notches
          notch_temp[i-26] = DigitChar2Int(current);
        
        ipfile.get(current); ipfile >> ws;
      }
      else // cuurent is digit, next is invalid
      {
        cerr << "Non-numeric character for mapping in rotor file rotor.rot" << endl;
        return NON_NUMERIC_CHARACTER;
      }
    }
    else if(IsWhiteSpace(current)) // current is ws
      ipfile >> ws;
    else // current is invalid
    {
      cerr << "Non-numeric character for mapping in rotor file rotor.rot" << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }
    
  //=== 3. Test if the file attemp to illegaly connect a contact and if it has notch positions
  if(i < 26)// less than 26 parameters in the config
  {
    cerr << "Not all inputs mapped in rotor file: rotor.rot" << endl;
    return INVALID_ROTOR_MAPPING;
  }

  if(!IsLegalContact(mapping_temp, notch_temp)) // has >=  26 parameters but has ilegal contacts 
   return INVALID_ROTOR_MAPPING;

  //=== 4. Everything's Done
  ipfile.close(); isConfigLoaded = true;
  
  for(int i = 0; i < 26; i++)
  {
    curRtMap[i] = mapping_temp[i] - i; // store the mapping in terms of relative positions 
    notch[i] = notch_temp[i];
  }
  
  return NO_ERROR;
}


/* Load the Starting position of the rotors */
int Rotor::LoadStartingPos(const char* rtStartPosFilename)
{
  //=== 1. Open the file giving rotor starting positions
  ifstream ipfile; ipfile.open(rtStartPosFilename);
  if(ipfile.fail())
  {
    cerr << "Cannot open rotor statring position file" << endl;
    exit(ERROR_OPENING_CONFIGURATION_FILE);
  }

  //=== 2. Read the staring positions
  char current, next; // current and next char read from the file
  ipfile >> ws; // filestream starts from first non-ws char

  int rtPos_temp[currentRotorNum-1]; // temp copy for rotor positions
  for(int i = 0; i < currentRotorNum; i++) rtPos_temp[i] = -1;
  
  for(int i = 0; !ipfile.eof(); i++)
  {
    ipfile.get(current); next = ipfile.peek();

    if(IsDigit(current))
    {
      if(IsDigit(next)) // current and next are digits: a two-digit number
      {
        // Test if this two-digit number is between 10 and 25
        if(current=='1' || (current=='2' && next>='0' && next<='5'))
        {
          rtPos_temp[i] = 10 * DigitChar2Int(current) + DigitChar2Int(next);
          
          ipfile.get(current); ipfile >> ws; // skip the "next" to get the one after
        }
        else
        {
          cerr << "INVALID_INDEX (not between 0 adnd 25) in rotor positions file rotor.pos" << endl;
          return INVALID_INDEX;
        }
      }
      else if(IsWhiteSpace(next)) // current is a digit, next is a ws: a one-digit number
      {
        rtPos_temp[i] = DigitChar2Int(current);     
        ipfile.get(current); ipfile >> ws;
      }
      else // cuurent is digit, next is invalid
      {
        cerr << "Non-numeric character in rotor positions file rotor.pos" << endl;
        return NON_NUMERIC_CHARACTER;
      }
    }
    else if(IsWhiteSpace(current)) // current is ws
      ipfile >> ws;
    else // current is invalid
    {
      cerr << "Non-numeric character in rotor positions file rotor.pos" << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }

  //=== 3. Check if there is a starting position for this rotor
  if(rtPos_temp[rotorLabel] == -1)
  {
    cerr << "No starting position for rotor " << rotorLabel
         << " in rotor position file: rotor.pos" << endl;
    return NO_ROTOR_STARTING_POSITION;
  }
  else
    startingPos = rtPos_temp[rotorLabel];    

  //=== 4. Everything's Done
  ipfile.close(); isStartingPosLoaded = true;
  
  return NO_ERROR;
}


/* Functionality 2: Rotate a  rotor */
void Rotor::Rotate()
{
  int temp = 0;
  for(int i = 0; i < 26; i++)
  {
    if(i == 0)
      temp = curRtMap[i];
    
    if(i < 25) curRtMap[i] = curRtMap[i+1];
    else curRtMap[i] = temp;
  }
  
  // Increment the top position of the rotor
  rotorTop++; rotorTop %= 26;
}


/* Set the Rotor's position to the designated starting position */
void Rotor::SetPosToStartingPos()
{
  if(isStartingPosLoaded) // only set if the staring position is given
  {
    for(int i = 0; i < 26; i++)
    {
      if(rotorTop != startingPos) Rotate(); // rotorTop've got updated as well
      else return;
    }
  }
  else return;
}


/* Check if there is a notch at the top absolute reference posiion, return true if so, false otherwise
*/
bool Rotor::IsNotchAtTop()
{
  for(int i = 0; i < 26; i++)
  {
    if(notch[i] == rotorTop) return true;
  }
  return false;
}


/* Rotate due to the rotor to its right whose top position hits a notch 
   Flag is the returned value of the IsNotchAtTop() of the rotor to its right */
void Rotor::RotateDueToNotch(bool flag)
{
  if(flag) Rotate();
  else return;
}


/* Functionality 1.1: Map the letters forwards: return if the rotor to its next should also rotate 
*/
void Rotor::MapForwards(char& ch)
{
  int ch0Based = ch - 'A'; // convert the lettter ch into 0-based from int
  if(isConfigLoaded)   
  {
    // The image of the mapping in 0-based form
    int image = (curRtMap[ch0Based]+ch0Based+26) % 26; // +26 to avoid negative before doing %
    ch = Letter0BasedInt2Char(image);
  }
  else return;
}


/* Functionality 1.2: Map the letters backwards */
void Rotor::MapBackwards(char& ch)
{
  int ch0Based = ch - 'A'; // convert the lettter ch into 0-based from int
  if(isConfigLoaded)   
  {
    int preimage = 0; // the preimage of the mapping in 0-based form
    for(int i = 0; i < 26; i++)
    {
      if((curRtMap[i]+i+26) % 26 == ch0Based)
      {
        preimage = i;
        break;
      }
    }
    
    ch = Letter0BasedInt2Char(preimage);
  }
  else return;
}


/* Destructor */
Rotor::~Rotor(){ currentRotorNum--; }
