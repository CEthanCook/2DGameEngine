#include "fallingSprite.h"
#include "gameData.h"
#include "imageFactory.h"

FallingSprite::FallingSprite(const std::string& name):
	MultiSprite(name),
	isDead(false),
	xMod(Gamedata::getInstance().getXmlInt(name + "/offset")),
	delayMod(Gamedata::getInstance().getXmlInt(name + "/delay")),
	delay(rand() % delayMod)
	{
		int xOffset = rand() % xMod;
		if (getX() + xOffset > worldWidth - getScaledWidth())
			setX(worldWidth - getScaledWidth());
		else
			setX(getX() + xOffset);
	}

void FallingSprite::draw() const{
	if (delay <= 0) {
		getImage()->draw(getX(), getY(), getScale());
	}
}
void FallingSprite::update(Uint32 ticks) {
	if (isDead) {
		delay = rand() % delayMod;
		setX(rand() % xMod);
		setY(0);
		setVelocityY(300);
		isDead = false;
		currentFrame = 0;
	}
	if (delay > 0) {
		delay -= ticks/2;
	}

	if (delay <= 0 && !isDead) {
	  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	  setPosition(getPosition() + incr);
	  if ( getY() < 0) {
	    setVelocityY( fabs( getVelocityY() ) );
	  }

	  if ( getY() > worldHeight-getScaledHeight()) {
			advanceFrame(ticks);
	    setVelocityY( 0 );
			if (currentFrame == numberOfFrames-1) {
				isDead = true;
			}
	  }

	  if ( getX() < 0) {
	    setVelocityX( fabs( getVelocityX() ) );
	  }
	  if ( getX() > worldWidth-getScaledWidth()) {
	    setVelocityX( -fabs( getVelocityX() ) );
	  }
	}

}
