#ifndef MATHUTILS_H
#define MATHUTILS_H

template <typename T, typename S>
double dot(const T& vectorA, const S& vectorB)
{
  return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
}

#endif // MATHUTILS_H

