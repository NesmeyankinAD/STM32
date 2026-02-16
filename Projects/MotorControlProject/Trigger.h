#pragma once

class Trigger
{
protected:
  bool quit;
  bool previous_state;

public:
  Trigger();

public:
  void set(bool expression);
  void reset(bool expression);

public:  
  bool get_quit();

};//Trigger