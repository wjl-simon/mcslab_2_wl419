/*
  Helper free functions for the enigma machine, class Plugboard, Rotor and Reflector
*/

#include "helper.h"

using namespace std;


/* Return true if given a white sapce, otherwise false */
bool IsWhiteSpace(char ch)
{
  if(ch==' ' || ch=='\t' || ch=='\n' || ch=='\v' || ch=='\f' || ch=='\r') return true;
  else return false;
}

/* Return true if given a digit(0-9), otherwise false */
bool IsDigit(char ch)
{
  if(ch >= '0' && ch <= '9') return true;
  else return false;
}


/* Convert 0-based letters (in char) into one Latin character (in char) e.g. '1''0' -> 'K' */
char Letter0Based2Char(char num) { return (char)(num - '0' + 'A'); }

char Letter0Based2Char(char tens, char ones)
{ return static_cast<char>(10 * (tens-'0') + (ones-'0') + 'A'); }


/* Convert digit (0-9) from char(e.g. '0','1','2') into int (e.g. 0,1,2) */
int DigitChar2Int(char digit) { return digit-'0'; }


/* Convert 0-based letters (int) into Latin letters (char), for Rotor class */
char Letter0BasedInt2Char(int num){ return static_cast<char>(num+'A'); }
