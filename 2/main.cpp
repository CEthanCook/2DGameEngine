#include <iostream>
#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL_ttf.h>
#include "drawFilledTriangle.h"
#include <vector>
#include <random>
#include "frameGenerator.h"
#include "filledTrapezoid.h"

const std::string TITLE = "llama";
const std::string NAME = "creigh3";
const int WIDTH = 640;
const int HEIGHT = 480;

struct Color{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
  Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
  Color operator + (int c) const {
    int red = (r+c>255) ? 255: r+c;
    int green = (g+c>255) ? 255: g+c;
    int blue = (b+c>255) ? 255: b+c;
    return Color(red ,green,blue, a);
  }
  Color operator / (unsigned char c) const {
    return Color(r/c, g/c, b/c, a);
  }
};
Color lBlue = {61, 195, 253, 200};
Color dBlue = {31, 116, 240, 200};
Color purple = {227, 134, 243, 200};
Color brown = {98,62,101,255};
Color white = {255, 255, 255, 255};
int random(int spread) {
  return rand()%(spread*2) - spread;
}
void setFromColor(SDL_Renderer* renderer, Color& c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}
void setFromColor(SDL_Renderer* renderer, Color& c, unsigned char alpha) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
}
void setFromColor(SDL_Renderer* renderer, Color&& c, unsigned char alpha) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, alpha);
}
void setFromColor(SDL_Renderer* renderer, Color&& c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}
Color randomizeColor(Color& c, int spread = 40) {
  Color newColor = c;
  int r = random(spread);
  newColor = newColor + r;
  return newColor;
}
void drawCircle(SDL_Renderer* renderer,
  Vector2f center, int radius, Color color) {
  setFromColor(renderer, color);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(renderer, center[0] + dx, center[1] + dy);
      }
    }
  }
}

void writeName(SDL_Renderer* renderer) {
  TTF_Init();
  TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  SDL_Color textColor = {0xff, 0, 0, 0};
  SDL_Surface* surface =
    TTF_RenderText_Solid(font, TITLE.c_str(), textColor);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {20, HEIGHT-40, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void drawLine(SDL_Renderer * r, Vector2f& p1, Vector2f& p2) {
  SDL_RenderDrawLine(r, p1[0], p1[1], p2[0], p2[1]);
}
void generatePaperStripes(SDL_Renderer *renderer, std::vector<Color>& colors, int x, int y, int width, int height, int wSpread = 10, int hSpread = 5, int count = 50, int numStripes = 6) {
  int paperLength = width/count;
  int bottomOffset = 3;
  FilledTrapezoid paper(renderer);
  std::vector<Vector2f> points;
  points.resize(4);
  int topSpread, bottomSpread, sideSpread = 0;
  height /= numStripes;
  Vector2f bottomRight, bottomLeft, topRight, topLeft;
  for (int i = 0; i < numStripes; i++) {
    topSpread = 0,bottomSpread = 0,sideSpread = 0;
    for (int j = 0; j < count; j++) {
      if (j != 0 && j != count-1) {
        topSpread = random(hSpread);
        bottomSpread = random(hSpread);
        sideSpread = random(wSpread);
      }
      points[0] = Vector2f(x + sideSpread - bottomOffset, y + height + bottomSpread);
      points[1] = Vector2f(x + paperLength + sideSpread + bottomOffset, y + height + bottomSpread);
      points[2] = Vector2f(x + sideSpread, y + topSpread);
      points[3] =  Vector2f(x + paperLength + sideSpread, y + topSpread);

      Color paperColor = randomizeColor(colors[i%colors.size()]);
      setFromColor( renderer, paperColor/2, 200);
      drawLine(renderer, points[0], points[1]);
      drawLine(renderer, points[0], points[2]);
      drawLine(renderer, points[1], points[3]);

      setFromColor( renderer, paperColor, 100);
      paper.setPoints(points);
      paper.draw();
      x += paperLength;
    }
    x -= paperLength * count;
    y += height;
  }
}
void drawStripedRectangle(SDL_Renderer *renderer, std::vector<Color>& colors, int x, int y, int width, int height, int numStripes) {
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = width;
  r.h = height/numStripes;
  for (int i = 0; i < numStripes; i++) {
    setFromColor(renderer, colors[i%colors.size()]);
    SDL_RenderFillRect( renderer, &r);
    SDL_RenderDrawRect( renderer, &r);
    r.y += r.h;
  }
}

void drawNeck(SDL_Renderer *renderer, int x, int y, int width, int height, int harnessWidth = 10) {
  SDL_Rect neck;
  neck.x = x;
  neck.y = y;
  neck.w = width;
  neck.h = height;
  setFromColor( renderer, purple, 255);
  SDL_RenderFillRect( renderer, &neck);
  SDL_RenderDrawRect( renderer, &neck);
  std::vector<Color> paperColor = { purple };
  generatePaperStripes( renderer, paperColor, x, y, width, height, 10, 5, 12);
  setFromColor( renderer, purple, 255);

  Vector2f bottomLeft(x + width - 2, y + height * .75);
  Vector2f bottomRight(x + width * 1.75, y + height/2);
  Vector2f topLeft(x + width, y);
  Vector2f topRight(x + width * 1.5, y + height/5);
  FilledTrapezoid trap(renderer, bottomLeft, bottomRight, topLeft, topRight);
  trap.draw();

  Vector2f top(x + width - 2, y + height*.75);
  Vector2f bottom(bottomRight[0], y + height*.75);
  Vector2f side(bottomRight[0] + 5, y + height*.75 - 15);
  DrawFilledTriangle mouth(renderer, top, bottom, side);
  mouth.draw();

  Vector2f bottomLeftEar(x + width * 2/3, y);
  Vector2f bottomRightEar(x + width, y);
  Vector2f topEar(x + width, y - height*.5);
  DrawFilledTriangle ear(renderer, bottomLeftEar, bottomRightEar, topEar);
  ear.draw();

  setFromColor(renderer, white);
  Vector2f topMouth = Vector2f(side[0], side[1] - 5);
  DrawFilledTriangle teeth(renderer, top, side, topMouth);
  teeth.draw();

  drawCircle(renderer, Vector2f(x+width*.8, y + height/3), 10, white);
  drawCircle(renderer, Vector2f(x+width*.8, y+ height/3), 3, Color(0,0,0,255));
  std::vector<Vector2f> harness = {
    Vector2f(x + width - harnessWidth, y + height * .75),
    Vector2f(x + width, y + height * .75),
    Vector2f(x + 1, y + harnessWidth),
    Vector2f(x, y)
  };
  setFromColor( renderer, brown);
  trap.setPoints(harness);
  trap.draw();
  std::vector<Vector2f> harness2 = {
    Vector2f(x + width, y + height*.75),
    Vector2f(x + width + harnessWidth, y + height*.75 - 5),
    Vector2f(x + width*1.5, y + height/5),
    Vector2f(x + width*1.5 + 5, y + height/5 + harnessWidth)
  };
  trap.setPoints(harness2);
  trap.draw();
}

void drawFeet(SDL_Renderer *renderer, int x, int y, int width, int height, int hoofSize = 10) {
  SDL_Rect foot;
  foot.x = x;
  foot.y = y;
  foot.w = width;
  foot.h = height;

  setFromColor( renderer, dBlue, 255);
  SDL_RenderFillRect( renderer, &foot);
  SDL_RenderDrawRect( renderer, &foot);

  setFromColor( renderer, brown);
  foot.h = hoofSize;
  foot.y = y + height - hoofSize;
  SDL_RenderFillRect( renderer, &foot);
  SDL_RenderDrawRect( renderer, &foot);
}

void drawBanner(SDL_Renderer *renderer, int x, int y, int width, int height) {
  SDL_Rect banner;
  banner.x = x;
  banner.y = y;
  banner.w = width;
  banner.h = height;
  Color outerBanner = { 197, 170, 157, 255};
  setFromColor( renderer, outerBanner);
  SDL_RenderFillRect( renderer, &banner);
  SDL_RenderDrawRect( renderer, &banner);

  banner.x = x + width/8;
  banner.y = y + height/8;
  banner.w = width * .75;
  banner.h = height * .75;
  Color innerBanner = {251, 239, 226, 255};
  setFromColor( renderer, innerBanner);
  SDL_RenderFillRect( renderer, &banner);
  SDL_RenderDrawRect( renderer, &banner);

  SDL_Rect arrow;
  arrow.x = x + width * 3/8;
  arrow.y = y + height * .5;
  arrow.w = width * .25;
  arrow.h = height * .25;
  setFromColor( renderer, brown);
  SDL_RenderFillRect( renderer, &arrow);
  SDL_RenderDrawRect( renderer, &arrow);

  Vector2f bottomRight(x + width * 1/4, y + height * .5);
  Vector2f bottomLeft(x + width * 3/4, y + height * .5);
  Vector2f top(x + width * .5, y + height * .25);
  DrawFilledTriangle topArrow( renderer, bottomRight, bottomLeft, top);
  topArrow.draw();
}
int main (int , char*[]) {
  srand(time(NULL));
  if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
    std::cout << "Failed to initialize SDL2" << std::endl;
    return EXIT_FAILURE;
  }
  SDL_Window* window = SDL_CreateWindow(
      "llama", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WIDTH, HEIGHT, SDL_WINDOW_SHOWN
  );

  SDL_Renderer* renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_SOFTWARE
  );

  //loading background
  SDL_Surface *surface = IMG_Load("background.png");
  if (surface == NULL) {
    std::cout << "Couldn't load background.jpg" << std::endl;
    return 1;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderClear( renderer );
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  //drawing feet
  drawFeet( renderer, 225, 275, 25, 75);
  drawFeet( renderer, 265, 250, 25, 75);
  drawFeet( renderer, 355, 250, 25, 75);
  drawFeet( renderer, 395, 275, 25, 75);

  //drawing neck and face
  drawNeck( renderer, 400, 75, 50, 75);
  std::vector<Color> colors = {
    lBlue, dBlue, purple
  };

  //drawing body
  int bodyX = 200;
  int bodyY = 150;
  int bodyLength = 250;
  int bodyHeight = 150;
  drawStripedRectangle(renderer, colors, bodyX, bodyY, bodyLength, bodyHeight, 6);

  lBlue.a = 50;
  dBlue.a = 50;
  purple.a = 50;
  generatePaperStripes( renderer, colors, bodyX, bodyY, bodyLength, bodyHeight);

  drawBanner( renderer, bodyX + bodyLength/2 -bodyLength/6, bodyY, bodyLength * 1/3, bodyHeight * 2/3);

  writeName(renderer);
  SDL_RenderPresent(renderer);
  FrameGenerator frameGen(renderer, window, WIDTH, HEIGHT, NAME);
  frameGen.makeFrame();
  SDL_RenderPresent(renderer);
  SDL_Event event;
  const Uint8* keystate;
  while ( true ) {
    keystate = SDL_GetKeyboardState(0);
    if (keystate[SDL_SCANCODE_ESCAPE]) { break; }
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
