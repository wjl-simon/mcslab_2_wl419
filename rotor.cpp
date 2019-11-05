#include <iostream>
#include <ifstream>
#include <cstdlib>
#include "errors.h"
#include "rotor.h"

using namespace std;

bool Rotor::IsWhiteSpace(char ch)
{
  if(ch==' ' || ch=='\t' || ch=='\n' || ch=='\v' || ch=='\f' || ch=='\r') return true;
  else return false;
}


bool Rotor::IsDigit(char ch)
{
  if(ch >= '0' && ch <= '9') return true;
  else return false;
}

bool Rotor::IsLegalContact()
{
  // It's legal means every digit in currentMap is unique
  for(int i = 0; i < 26; i++)
    for(int j = i+1; j < 26; j++)
      if(currentMap[i] == currentMap[j])
        return false;
  // Notch postions should also be unique
  for(int i = 0; i < 26 && notchPos[i]!=-1; i++)
    for(int j = i+1; j < 26 && notchPos[j]!=-1; j++)
      if(notchPos[i] == notchPos[j])
        return false;
  return true;
}


/* Convert 0-based letters () into Latin letters (char) */
char Letter0Based2char(int num){ return static_cast<char>(num+65); }


/* Convert digit (0-9) from char('0','1','2') into int (0,1,2) */
int Rotor::DigitChar2Int(char digit) { return digit-48; }



/* Functionality 2: Rotate the rotor */
void Rotor::Rotate()
{
  for(int i = 0; i < 26; i++)
    currentMap[i] = (currentMap[i] + 1) % 26;
}


/* Default Constructor*/
Rotor::Rotor(): currentRotorNum(0), startingPos(-1)
{
  for(int i = 0; i < 26; i++){notchPos[i] = -1; currentMap[i] = -1;}
  currentRotorNum++; // increment by 1 for each new rotor instance
  rotorLabel = numRotor;
  // Kill this new instances if there're already 3 of them
  //if(numRotors > TOTAL_ROTOR_NUM) ~Rotor();
}


/* Load the configuration for this rotor  */
int Rotor::LoadConfig(const char* rtConfigFilename)
{
  //=== 1. Open the file giving rotor configuration
  ifstream ipfile; ipfile.open(rtConfigFilename);
  if(ipfile.fail())
  {
    cerr << "Cannot open rotor configuration file" << endl;
    exit(-1);
  }

  //=== 2. Get the numbers (the letters) from the texture file
  char current, next; // current and next char read from the file
  ipfile >> ws; // filestream starts from first non-ws char
  int mapping_temp[26], notchPos_temp[26]; // temporary copies for the currentMap and notchPos
 
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
            notchPos_temp[i-26] = 10 * DigitChar2Int(current) + DigitChar2Int(next);
          ipfile.get(current); ipfile.get(current); // skip the "next" to get the one after
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
          notchPos_temp[i-26] = DigitChar2Int(current);
        ipfile >> ws; ipfile.get(current);
      }
      else // cuurent is digit, next is invalid
      {
        cerr << "NON_NUMERIC_CHARACTER in the rotor config!" << endl;
        return NON_NUMERIC_CHARACTER;
      }
    }
    else if(IsWhiteSpace(current)) // current is ws
    {
      ipfile >> ws;
      ipfile.get(current);
    }
    else // current is invalid
    {
      cerr << "NON_NUMERIC_CHARACTER in the rotor config!" << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }
  
  //=== 3. Test if the file attemp to illegaly connect a contact and if it has notch positions

  if(!IsLegalContact())
  {
    cerr << "INVALID_ROTOR_MAPPING in the rotor config!" << endl;
    return INVALID_ROTOR_MAPPING;
  }

  //=== 4. Everything's Done
  ipfile.close(); isLoaded = true;
  for(int i = 0; i < 26; i++)
  {
    currentMap[i] = mapping_temp[i];
    notchPos[i] = notchPos_temp[i];
  } 
  return NO_ERROR;
}


/* Load the Starting position of the rotors */
static int Rotor::LoadStartingPos(const char* rtStartPosFilename)
{
  //=== 1. Open the file giving rotor starting positions
  ifstream ipfile; ipfile.open(rtStartPosFilename);
  if(ipfile.fail())
  {
    cerr << "Cannot open rotor statring position file" << endl;
    exit(-1);
  }

  //=== 2. Read the staring positions
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
          if(i == rotorLabel)
          {
            startingPos = 10 * DigitChar2Int(current) + DigitChar2Int(next); break;
          }
          ipfile.get(current); ipfile.get(current); // skip the "next" to get the one after
        }
        else
        {
          cerr << "NON_NUMERIC_CHARACTER in the rotor starting position!" << endl;
          return NON_NUMERIC_CHARACTER;
        }
      }
      else if(IsWhiteSpace(next)) // current is a digit, next is a ws: a one-digit number
      {
        if(i == rotorLabel)
        {
          startingPos = DigitChar2Int(current); break;
        }
        ipfile >> ws; ipfile.get(current);
      }
      else // cuurent is digit, next is invalid
      {
        cerr << "NON_NUMERIC_CHARACTER in the rotor starting position!" << endl;
        return NON_NUMERIC_CHARACTER;
      }
    }
    else if(IsWhiteSpace(current)) // current is ws
    {
      ipfile >> ws;
      ipfile.get(current);
    }
    else // current is invalid
    {
      cerr << "NON_NUMERIC_CHARACTER in the rotor starting position!" << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }

  //=== 3. Check if there is a starting position  
  if(i != rotorLabel)
  {
    cerr << "NO_ROTOR_STARTING_POSITION" << endl;
    return NO_ROTOR_STARTING_POSITION;
  }
  
  //=== 4. Everything's Done
  ipfile.close(); return NO_ERROR;
}


/* Set the Rotor's position to the designated starting position */
void Rotor::SetPosToStartingPos()
{
  if(startingPos != -1) // only allow to set if the staring position is given
    for(int i = 0; i < 25; i++)
      if(currentMap[0]!=startingPos) Rotate();
      else break;
  else return;
}


/* Functionality 1: Map the letters */
void Rotor::DoMapping(char& ch)
{
  int index = ch - 65; // convert the lettter ch into 0-based from int

  // When a key is pressed a rotation happens before closing the circuit
  for(int i = 0; notchPos[i]!=-1 && notchPos < 26; i++) // check if needing rotation
    if(notchPos[i] >= 0 && notchPos[i] <= 25) // a valid notch
    {
      if(notchPos[i] == index)
      {
        Rotate(); break;
      }
    }
    else return; // do nothing if loading failed

  // Cicuit close, perform the mapping
  ch = Letter0baed2char(currentMap[index]);
}


/* Destructor */
Rotor::~Rotor(){ currentRotorNum--; }
