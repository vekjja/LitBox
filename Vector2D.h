#ifndef VECTOR2D_H
#define VECTOR2D_H

struct Vector2D {
  float x, y;
};

Vector2D makeVector(float x, float y) {
  Vector2D v;
  v.x = x;
  v.y = y;
  return v;
}

Vector2D subtract(Vector2D v1, Vector2D v2) {
  return makeVector(v1.x - v2.x, v1.y - v2.y);
}

float dotProduct(Vector2D v1, Vector2D v2) { return v1.x * v2.x + v1.y * v2.y; }

Vector2D scaleVector(Vector2D v, float scale) {
  return makeVector(v.x * scale, v.y * scale);
}

float vectorLength(Vector2D v) { return sqrt(v.x * v.x + v.y * v.y); }

Vector2D normalize(Vector2D v) {
  float length = vectorLength(v);
  if (length != 0) {
    return scaleVector(v, 1.0 / length);
  }
  return v;
}

#endif