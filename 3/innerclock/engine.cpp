#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twoWayMultiSprite.h"
#include "fallingSprite.h"
#include "gameData.h"
#include "engine.h"
#include "clock.h"
#include "frameGenerator.h"

Engine::~Engine() {
  for (auto& obj : objects) {
    delete obj;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  skyline("skyline", Gamedata::getInstance().getXmlInt("skyline/factor") ),
  buildings("buildings", Gamedata::getInstance().getXmlInt("buildings/factor") ),
  floor("floor", Gamedata::getInstance().getXmlInt("floor/factor") ),
  viewport( Viewport::getInstance() ),
  objects( { new TwoWayMultiSprite("Player"), new Sprite("Platform"), new Sprite("Platform2")}),
  bombCount(Gamedata::getInstance().getXmlInt("Bomb/count")),
  currentSprite(0),
  makeVideo( false )
{
  for (int i = 0; i < bombCount; i++) {
    objects.emplace_back(new FallingSprite("Bomb"));
  }
  Viewport::getInstance().setObjectToTrack(objects[0]);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  skyline.draw();
  buildings.draw();
  floor.draw();

  for (auto& obj : objects) {
    obj->draw();
  }

  clock.draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for (auto& obj : objects) {
    obj->update(ticks);
  }

  skyline.update();
  buildings.update();
  floor.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  Viewport::getInstance().setObjectToTrack(objects[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
