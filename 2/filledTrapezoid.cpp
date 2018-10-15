#include "drawFilledTriangle.h"
#include "filledTrapezoid.h"

FilledTrapezoid::FilledTrapezoid(SDL_Renderer* r,
                  const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4)
  : renderer(r), v0(p1), v1(p2), v2(p3), v3(p4){}

FilledTrapezoid::FilledTrapezoid(SDL_Renderer* r)
  : renderer(r){}

void FilledTrapezoid::draw() const {
  DrawFilledTriangle( renderer, v0, v1, v2).draw();
  DrawFilledTriangle( renderer, v1, v2, v3).draw();
}

void FilledTrapezoid::setPoints(std::vector<Vector2f>& points) {
  if (points.size() != 4) return;
  v0 = points[0];
  v1 = points[1];
  v2 = points[2];
  v3 = points[3];
}

std::ostream& operator<<(std::ostream& out, const FilledTrapezoid& ft) {
  return out << "v0: " << ft.v0 << "\nv1: " << ft.v1 << "\nv2: " << ft.v2 << "\nv3: " << ft.v3 << std::endl;
}
