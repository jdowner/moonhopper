#include "PeriodicDomain.h"
#include <cassert>

PeriodicDomain::PeriodicDomain(
  double minx, double maxx, double miny, double maxy) :
  m_minx(minx),
  m_maxx(maxx),
  m_miny(miny),
  m_maxy(maxy),
  m_diffx(m_maxx - m_minx),
  m_diffy(m_maxy - m_miny)
{
  assert(maxx > minx);
  assert(maxy > miny);
}

double PeriodicDomain::toX(double x) const
{
  while (x < m_minx) x += m_diffx;
  while (x > m_maxx) x -= m_diffx;
  return x;
}

double PeriodicDomain::toY(double y) const
{
  while (y < m_miny) y += m_diffy;
  while (y > m_maxy) y -= m_diffy;
  return y;
}

double PeriodicDomain::getMinX() const
{
  return m_minx;
}

double PeriodicDomain::getMaxX() const
{
  return m_maxx;
}

double PeriodicDomain::getMinY() const
{
  return m_miny;
}

double PeriodicDomain::getMaxY() const
{
  return m_maxy;
}

