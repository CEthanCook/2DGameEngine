#include <iostream>
#include <SDL2/SDL.h>
#include "vector2f.h"
#include <vector>

class FilledTrapezoid {
public:
  FilledTrapezoid(SDL_Renderer*);
  FilledTrapezoid(SDL_Renderer*,
                  const Vector2f&, const Vector2f&, const Vector2f&, const Vector2f&);
  void draw() const;
  void setPoints(std::vector<Vector2f>& points);
private:
  SDL_Renderer* renderer;
  Vector2f v0;
  Vector2f v1;
  Vector2f v2;
  Vector2f v3;
  friend std::ostream& operator<<(std::ostream& out, const FilledTrapezoid& ft);
};
