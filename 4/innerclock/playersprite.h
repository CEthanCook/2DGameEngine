#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

enum class PlayerStates {
	kIdle = 0
	kMoveLeft,
	kMoveRight,
	kJump,
	kDead
}

class PlayerSprite : public Drawable {
public:
  PlayerSprite(const std::string&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual inline const Image* getImage() const {
    return images[currentFrame].second;
  }
  inline int getScaledWidth()  const {
    return getScale()*getImage()->getWidth();
  }
  inline int getScaledHeight()  const {
    return getScale()*getImage()->getHeight();
  }
  virtual inline const SDL_Surface* getSurface() const {
    return getImage()->getSurface();
  }

protected:
  std::map<PlayerStates::kIdle, std::vector<Image *>> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&);
};
#endif
