#include "AABB.h"

AABB::AABB(double minx, double miny, double maxx, double maxy)
: m_minx(minx)
, m_miny(miny)
, m_maxx(maxx)
, m_maxy(maxy)
{
}

bool AABB::contains(double x, double y) const
{
  return ((m_minx <= x) && (x <= m_maxx) ) && ((m_miny <= y) && (y <= m_maxy));
}
