#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    if (isLeft) {
      return leftImages[currentFrame];
    }
    return rightImages[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*getImage()->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*getImage()->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return getImage()->getSurface();
  }

protected:
  std::vector<Image *> leftImages;
  std::vector<Image *> rightImages;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  int isLeft;
  void advanceFrame(Uint32 ticks);
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
};
#endif
