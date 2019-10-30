#ifndef ROTOR_H
#define ROTOR_H

class Rotor
{
 public:
  /* Load the configuration for this rotor  */
  int LoadConfig(const char* filename);
  void Mapping(char& ch);
  void Rotate(char& ch);
};


#endif
