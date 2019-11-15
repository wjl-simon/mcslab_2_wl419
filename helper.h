/*
  Helper free functions for the enigma machine, class Plugboard, Rotor and Reflector
*/

#ifndef HELPER_H
#define HELPER_H

/* Return true if given a white sapce, otherwise false */
bool IsWhiteSpace(char ch);

/* Return true if given a digit(0-9), otherwise false  */
bool IsDigit(char ch);


/* Convert 0-based letters (in char) into one Latin character (in char) e.g. '1''0' -> 'K' */
char Letter0Based2Char(char num); // convert from 1-digit num
char Letter0Based2Char(char tens, char ones); // convert from 2-digit num


/* Convert digit (0-9) from char(e.g. '0','1','2') into int (e.g. 0,1,2) */
int DigitChar2Int(char digit);

/* Convert 0-based letters (int) into Latin letters (char), for Rotor class */
char Letter0BasedInt2Char(int num);



#endif
