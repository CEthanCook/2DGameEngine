#ifndef PFALLINGSPRITE__H
#define FALLINGPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "multisprite.h"

class FallingSprite : public MultiSprite {
public:
  FallingSprite(const std::string&);

  virtual void draw() const;
  virtual void update(Uint32);
protected:
  bool isDead;
  int xMod;
  int delayMod;
  float delay;

};
#endif
