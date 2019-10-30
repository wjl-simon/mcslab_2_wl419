#include <iostream>
#include <fstream>
#include "errors.h"
#include <cstdlib>

using namespace std;

/* Return true if given a white sapce, otherwise false */
bool PLugboard::IsWhiteSpace(char ch)
{
  if(ch==' ' || ch=='\t' || ch=='\n' || ch=='\v' || ch=='\f' || ch=='\r') return true;
  else return false;
}


/* Return true if given a digit(0-9), otherwise false  */
bool IsDigit(char ch)
{
  if(ch >= '0' && ch <= '9') return true;
  else return false;
}


/* Return false if the file connects a contact with itself or with more than one other  */
bool IsLegalContact()
{
   // for(int i = 0; i <= letterNum-2; i=i+2)
   // {
   //   // Check if connecting with itself
   //   if(letters[i] == letters[i+1])
   //     return false;
   //   else
   //   {
   //    // Check if a contact conncts with more than one other contact
   //     for(int j = i+2; j < letterNum-2; j=j+2)
   //       if(letters[i]==letters[j] || letters[i]==letters[j+1] ||
   //          letters[i+1]==letters[j] || letters[i+1]==letters[j+1])
   //         return false;
   //   }
   // }
  // It's legal means every digit is unique
  for(int i = 0; i < letterNum; i++)
    for(int j = i+1; j < letterNum; j++)
      if(letters[i] == letters[j])
        return false;
    
  return true;
}


/* Default constructor */
Plugboard:: Plugboard(): letterNum(0), isLoaded(false)
{
}


/* Load the plugboard configuration */
int Plugboard::LoadConfig(char* pbConfigFileName)
{
  //=== 1. Open the file giving plugboard configuration
  ifstream ipfile; ipfile.open(pbConfigFileName);
  if(ipfile.fail())
  {
    cerr << "Cannot open plugboard configuration file" << endl;
    exit(1);
  }

  
  //=== 2. Get the numbers (the letters) from the texture file
  char current, next; // current and next char from the file
  ipfile >> ws; // filestream starts from first non-ws char

  int i;
  for(i = 0; i < 26 && !ipfile.eof(); i++)
  {
    ipfile.get(current); next = ipfile.peek();

    if(IsDigit(current))
    {
      if(IsDigit(next)) // current and next are digits: a two-digit number
      {
        // Test if this two-digit number is between 10 and 25
        if(current=='1' || (current=='2' && next>='0' && next<='5'))
        {
          letters[i] = 10 * static_cast<int>(current) + static_cast<int>(next);
          ipfile.get(current); ipfile.get(curent); // skip the "next" to get the one after          
        }
        else
        {
          cerr << "INVALID INDEX (not between 0 and 25) in the plugboard config!" << endl;
          return INVALID INDEX;
        }
      }
      else if(IsWhiteSpace(next)) // current is digit, next is ws: a one-digit number
      {
        letters[i] = static_cast<int>(current);
        ipfile >> ws; ipfile.get(current);
      }
      else // cuurent is digit, next is invalid
      {
        cerr << "NON_NUMERIC_CHARACTER in the plugboard config!" << endl;
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
      cerr << "NON_NUMERIC_CHARACTER in the plugboard config!" << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }

  //=== 3. Test if the number of numbers is even
  letterNum = i; // if the program runs here then the numbers in the file shall be valid
  if(letterNum % 2 != 0)
  {
    cerr << "INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS (odd number of numbers) "
         << "in the plugboard config!" << endl;
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }

  //=== 4. Test if the file attemp to connect a contact to itself
  if(!IsLegalContact())
  {
    cerr << "IMPOSSIBLE_PLUGBOARD_CONFIGURATION in the plugboard config!" << endl;
    return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
  }
    
  //=== 5. Everything's Done
  ipfile.close();
  isLoaded = true;
  return NO_ERROR;
}


/* Swap the letters (the functionality of plugboard) */
void Plugboard::SwapLetters(char& letter)
{
  if(isLoaded) // if sucessfully loaded
  {
    for(int i = 0; i <= letterNum - 2; i=i+2)
      if(letter == letterNum[i])
        letter = letterNum[i+1];
      else if(letter == letterNum[i+1])
        letter = letterNum[i];
  }
  else return; // do nothing if loading is unsucesful
}
