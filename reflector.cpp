#include <iostream>
#include <fstream>
#include "errors.h"
#include <cstdlib>
#include "reflector.h"

using namespace std;


/* Return true if given a white sapce, otherwise false */
bool Reflector::IsWhiteSpace(char ch)
{
  if(ch==' ' || ch=='\t' || ch=='\n' || ch=='\v' || ch=='\f' || ch=='\r') return true;
  else return false;
}


/* Return true if given a digit(0-9), otherwise false  */
bool Reflector::IsDigit(char ch)
{
  if(ch >= '0' && ch <= '9') return true;
  else return false;
}


/* Return false if the file connects a contact with itself or with more than one other  */
bool Reflector:: IsLegalContact()
{
  // It's legal means every digit is unique
  for(int i = 0; i < 26; i++) // should have exacly 13 pairs of digits
    for(int j = i+1; j < 26; j++)
      if(letters[i] == letters[j])
        return false;
    
  return true;
}


/* Convert 0-based letters (char) into Latin letters(char) */
char Reflector::Letter0Based2Char(char num) { return (char)(num - 48 + 65); }

char Reflector::Letter0Based2Char(char tens, char ones)
{
  return (char)(10 * (tens-48) + (ones-48) + 65);
}


/* Default constructor */
Reflector(): isLoaded(false)
{
  for(int i = 0; i < 26; i++) letters[i] = '?';
}

/* Load the plugboard configuration */
int Plugboard::LoadConfig(const char* rfConfigFileName)
{
  //=== 1. Open the file giving reflector configuration
  ifstream ipfile; ipfile.open(rfConfigFileName);
  if(ipfile.fail())
  {
    cerr << "Cannot open plugboard configuration file" << endl;
    exit(-1);
  }

  
  //=== 2. Get the numbers (the letters) from the texture file
  char current, next; // current and next char from the file
  ipfile >> ws; // filestream starts from first non-ws char
  char letters_temp[26]; // temporary copy for the letters array

  int i; // counter
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
          letters_temp[i] = Letter0Based2Char(current,next);
          ipfile.get(current); ipfile.get(current); // skip the "next" to get the one after 
        }
        else
        {
          cerr << "NON_NUMERIC_CHARACTER (not between 0 and 25) in the reflector config!" << endl;
          return NON_NUMERIC_CHARACTER;
        }
      }
      else if(IsWhiteSpace(next)) // current is digit, next is ws: a one-digit number
      {
        letters_temp[i] = Letter0Based2Char(current);
        ipfile >> ws; ipfile.get(current);
      }
      else // cuurent is a digit, next is invalid
      {
        cerr << "NON_NUMERIC_CHARACTER in the reflector config!" << endl;
        return NON_NUMERIC_CHARACTER;
      }
    }
    else if(IsWhiteSpace(current)) // current is a ws
    {
      ipfile >> ws;
      ipfile.get(current);
    }
    else // current is invalid
    {
      cerr << "NON_NUMERIC_CHARACTER in the reflector config!" << endl;
      return NON_NUMERIC_CHARACTER;
    }
  }

  //=== 3. Test if the number of numbers is 26 (13 pairs)
  //letterNum = i; // if the program runs here then the numbers in the file shall be valid
  if(i != 26)
  {
    cerr << "INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS (26 number of numbers) "
         << "in the reflector config!" << endl;
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }

  //=== 4. Test if the file attemp to connect a contact to itself or more than one other
  if(!IsLegalContact())
  {
    cerr << "INVALID REFLECTOR MAPPING in the reflector config!" << endl;
    return INVALID REFLECTOR MAPPING;
  }
    
  //=== 5. Everything's Done
  ipfile.close(); isLoaded = true;
  for(int i = 0; i < 26; i++) letters[i] == letters_temp[i]; // write on the letters arrray 
  return NO_ERROR;
}


/* Swap  letters (the functionality of reflector) */
void Reflector::SwapLetters(char& letter)
{
  if(isLoaded) // if sucessfully loaded
  {
    for(int i = 0; i <= 26 - 2; i=i+2) // the 13th pair is element number 24 and 25
      if(letter == letters[i])
      {
        letter = letters[i+1];
        return;
      }
      else if(letter == letters[i+1])
      {
        letter = letters[i];
        return;
      }
    // Runs here means there's no mapping for this letter: maps to itself
  }
  else return; // do nothing if loading failed
}
