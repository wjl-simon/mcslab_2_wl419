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
    for(int j = i+1; j < 26; j++)
      if(mapping[i] == mapping[j])
        return false;
  
  // Notch postions should also be unique
  for(int i = 0; i < 26 && notch[i]!=-1; i++)
    for(int j = i+1; j < 26 && notch[j]!=-1; j++)
      if(notch[i] == notch[j])
        return false;
  return true;
}


/* Functionality 2: Rotate the rotor */
void Rotor::Rotate()
{
  int temp = 0;
  for(int i = 25; i >= 0; i--)
  {
    if(i == 25)
      temp = letterAtAbsPos[i];
    
    if(i > 0) letterAtAbsPos[i] = letterAtAbsPos[i-1];
    else letterAtAbsPos[i] = temp;
  }
  // cout << "rotor " << rotorLabel << " rotates! Now the pos is:" << endl;
  // for(int i =0; i<26;i++)
  //   cout << letterAtAbsPos[i] << ' ';
  // cout << endl;
}


/* Constructors*/
Rotor::Rotor(): isConfigLoaded(false),isStartingPosLoaded(false),startingPos(-1)
{
  for(int i = 0; i < 26; i++){ letterAtAbsPos[i] = i; mapAbs2Abs[i] = -1; notch[i] = -1; }
  currentRotorNum++; // increment by 1 for each new rotor instance
  rotorLabel = currentRotorNum - 1; // the rotorLabel starts from 0
  //  cout << "This is rotor " << rotorLabel << endl;
}


Rotor::Rotor(const char* rtConfigFilename, const char* rtStartingPosilename):
  isConfigLoaded(false),isStartingPosLoaded(false),startingPos(-1)
{
  for(int i = 0; i < 26; i++){ letterAtAbsPos[i] = i; mapAbs2Abs[i] = -1; notch[i] = -1; }
  currentRotorNum++; // increment by 1 for each new rotor instance
  rotorLabel = currentRotorNum - 1; // the rotorLabel starts from 0

  // Load configs
  int e = this->LoadConfig(rtConfigFilename); if(e) exit(e);
  e = this->LoadStartingPos(rtStartingPosilename); if(e) exit(e);

  // Set starting position of the rotor
  this->SetPosToStartingPos();
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
  int mapping_temp[26]; // mapping between absolue positions e.g. from 12 o'clock to 5 o'clock
  int notch_temp[26]; // // an array of letters who have notches
  for(int i = 0; i < 26; i++){ mapping_temp[i] = -1; notch_temp[i] = -1; }
  
  int i; // counter
  for(i = 0; i < 52 && !ipfile.eof(); i++)
  {
    ipfile.get(current); next = ipfile.peek();

    if(IsDigit(current))
    {
      if(IsDigit(next)) // current and next are digits: a two-digit number
      {
        // Test if this two-digit number is between 10 and 25
        if(current=='1' || (current=='2' && next>='0' && next<='5'))
        {
          if(i < 26)
            mapping_temp[i] = 10 * DigitChar2Int(current) + DigitChar2Int(next); 
          else
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
        if(i < 26)
          mapping_temp[i] = DigitChar2Int(current);
        else
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

  if(!IsLegalContact(mapping_temp, notch_temp))
  {
    if(i < 26)// less than 26 parameters
      cerr << "Not all inputs mapped in rotor file: rotor.rot" << endl;
    else // more than 26 parameters
      cerr << "Invalid mapping of input 13 to output 3"
           <<"(output 3 is already mapped to from input 6) in"
           << endl;
    return INVALID_ROTOR_MAPPING;
  }

  //=== 4. Everything's Done
  ipfile.close(); isConfigLoaded = true;
  for(int i = 0; i < 26; i++)
  {
    mapAbs2Abs[i] = mapping_temp[i];
    notch[i] = notch_temp[i]; 
  }
  // cout << "rotor " << rotorLabel << " after config its Abs2Abs mapping:" << endl;
  // for(int i = 0; i<26; i++)
  //   {
  //     cout << mapAbs2Abs[i] << ' ';
  //   }
  // cout << endl << " And its notches are:" <<endl;
  // for(int i = 0; i<26; i++)
  //   {
  //     cout << notch[i] << ' ';
  //   }
  // cout << endl << endl;
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

  int rtPos_temp[currentRotorNum-1];
  for(int i = 0; i < currentRotorNum; i++) rtPos_temp[i] = -1;
  
  int i; // counter
  for(i = 1; i <= currentRotorNum  && !ipfile.eof(); i++)
  {
    ipfile.get(current); next = ipfile.peek();

    if(IsDigit(current))
    {
      if(IsDigit(next)) // current and next are digits: a two-digit number
      {
        // Test if this two-digit number is between 10 and 25
        if(current=='1' || (current=='2' && next>='0' && next<='5'))
        {
          // if(i == rotorLabel)
          // {
          //   startingPos = 10 * DigitChar2Int(current) + DigitChar2Int(next); break;
          //}
          rtPos_temp[i] = 10 * DigitChar2Int(current) + DigitChar2Int(next);
          
          ipfile.get(current); ipfile >> ws; // skip the "next" to get the one after
        }
        else
        {
          cerr << "Non-numeric character in rotor positions file rotor.pos" << endl;
          return NON_NUMERIC_CHARACTER;
        }
      }
      else if(IsWhiteSpace(next)) // current is a digit, next is a ws: a one-digit number
      {
        // if(i == rotorLabel)
        // {
        //   startingPos = DigitChar2Int(current); break;
        // }
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

  //=== 3. Check if there is a starting position  
  if(rtPos_temp[rotorLabel] == -1)
  {
    cerr << "No starting position for rotor " << rotorLabel
         << " in rotor position file: rotor.pos" << endl;
    return NO_ROTOR_STARTING_POSITION;
  }
  else startingPos = rtPos_temp[rotorLabel];
         
  //=== 4. Everything's Done
  ipfile.close(); isStartingPosLoaded = true;
  return NO_ERROR;
}


/* Set the Rotor's position to the designated starting position */
void Rotor::SetPosToStartingPos()
{
  if(isStartingPosLoaded) // only allow to set if the staring position is given
    for(int i = 0; i < 25; i++)
      if(letterAtAbsPos[0]!=startingPos){
        //cout << "setting!" << endl;
        Rotate();}
      else
        {
          //cout << "after rotation the cuurent pos is:" << endl;
          //for(int j = 0; j < 26; j++) cout << letterAtAbsPos[j] << ' ';
          //cout << endl;
          return;
        }
        
  else return;
}


/* Functionality 1.1: Map the letters forwards: return if the rotor to its next should also rotate 
*/
bool Rotor::MapForwards(char& ch)
{
  int ch0Based = ch - 65; // convert the lettter ch into 0-based from int
  if(isConfigLoaded)   
  {
    // When a key is pressed a rotation happens at the rightmost rotor before closing the circuit
    if(rotorLabel == 0)
      Rotate();
    
    // Cicuit close, perform the mapping
    int preimageAbsPos = 0; // abs pos of the source letter mapping from
    for(int i = 0; i < 26; i++)
      if(letterAtAbsPos[i] == ch0Based) // locate the Abs pos of ch
      {
        preimageAbsPos = i;
        break;
      }
    int imageAbsPos = mapAbs2Abs[preimageAbsPos]; // abs pos of the destination letter
    ch = Letter0BasedInt2Char(letterAtAbsPos[imageAbsPos]);

    //cout << "Rotor " << rotorLabel << " maps forwards into " << ch << endl; 
    
    // Check if there is a notch at the top absolute reference position
    for(int i = 0; notch[i]!=-1 && i < 26; i++)
      if(notch[i] >= 0 && notch[i] <= 25)
        if(notch[i] == letterAtAbsPos[0])
          return true; // telling the rotor to the left that it should also rotate

    return false;
  }
  else return false ;
}


/* Functionality 1.2: Map the letters backwards */
void Rotor::MapBackwards(char& ch)
{
  int ch0Based = ch - 65; // convert the lettter ch into 0-based from int
  
  if(isConfigLoaded)   
  {
    int imageAbsPos = 0; // the Abs pos of the image (destination character being mapped into)
    int preimageAbsPos = 0; // the Abs pos of the preimage (source character mapping from)
    for(int i = 0; i < 26; i++)
      if(letterAtAbsPos[i] == ch0Based) // locate the abs pos of the ch (image)
      {
        imageAbsPos = i;
        break;
      }
    
    for(int i = 0; i < 26; i++) // locate the abs pos of the preimage of ch
      if(imageAbsPos == mapAbs2Abs[i])
      {
        preimageAbsPos = i;
        break;
      }

    ch = Letter0BasedInt2Char(letterAtAbsPos[preimageAbsPos]);
    //cout << "Rotor " << rotorLabel << " maps backwards into " << ch << endl;
  }
  else return;
}


/* Rotate due to the rotor to its right whose absolute position hit a notch 
   Flag is the returned value of the DoMapping() member of the rotor to its right */
void Rotor::RotateDueToNotch(bool flag)
{
  if(flag) Rotate();
  else return;
}


/* Destructor */
Rotor::~Rotor(){ currentRotorNum--; }
