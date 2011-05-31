#include "PeriodicSpatialTree.h"
#include <cassert>
#include <cmath>
#include "Moon.h"
#include "CollisionResolution.h"

PeriodicSpatialTree::PeriodicSpatialTree(const PeriodicDomain& domain,
  double diameter) :
  m_domain(domain),
  m_diameter(diameter)
{
  m_width = m_domain.getMaxX() - m_domain.getMinX();
  m_height = m_domain.getMaxY() - m_domain.getMinY();

  m_cols = static_cast<unsigned int>(m_width / m_diameter);
  m_rows = static_cast<unsigned int>(m_height / m_diameter);

  m_dx = m_width / m_cols;
  m_dy = m_height / m_rows;
}

void PeriodicSpatialTree::add(Moon* moon)
{
  const unsigned int hash = calculateHash(moon);

  assert(m_data.find(hash) == m_data.end());
  m_data.insert(std::make_pair(hash,moon));
}

unsigned int PeriodicSpatialTree::calculateHash(Moon* moon) const
{
  // The hash is based upon the top-left corner of the moons bounding box. This
  // reduces the number of collisions that we need to check for
  const double relX = m_domain.toX(moon->x - moon->r) - m_domain.getMinX();
  const double relY = m_domain.toY(moon->y - moon->r) - m_domain.getMinY();

  assert(relX >= 0.0);
  assert(relY >= 0.0);

  const unsigned int i = floor(relX / m_dx);
  const unsigned int j = floor(relY / m_dy);

  return i + m_cols * j;
}

void PeriodicSpatialTree::clear()
{
  m_data.clear();
}

void PeriodicSpatialTree::resolveCollisions()
{
  typedef std::map<unsigned int, Moon*>::iterator Iterator;

  for(Iterator it = m_data.begin(); it != m_data.end(); ++it)
  {
    Moon* moon = it->second;

    unsigned int hash = it->first;
    unsigned int i = hash % m_cols;
    unsigned int j = hash / m_cols;

    resolveCollisionWith(moon, i + 1, j);
    resolveCollisionWith(moon, i, j + 1);
    resolveCollisionWith(moon, i + 1, j + 1);
  }
}

void PeriodicSpatialTree::resolveCollisionWith(
  Moon* moon, unsigned int i, unsigned int j)
{
  i = i % m_cols;
  j = j % m_rows;

  const unsigned int hash = i + m_cols * j;

  std::map<unsigned int, Moon*>::iterator result = m_data.find(hash);
  if (m_data.end() != result)
  {
    elasticCollision(m_domain, *moon, *result->second);
//    Moon* other = result->second;
//
//    const double r = moon->r + other->r;
//
//    double dx = other->x - moon->x;
//    double dy = other->y - moon->y;
//
//    if (dx < -m_diameter)
//    {
//      dx += m_width;
//    }
//
//    if (dx > m_diameter)
//    {
//      dx -= m_width;
//    }
//
//    if (dy < -m_diameter)
//    {
//      dy += m_height;
//    }
//
//    if (dy > m_diameter)
//    {
//      dy -= m_height;
//    }
//    
//    assert(dx < 2.0 * m_diameter);
//    assert(dy < 2.0 * m_diameter);
//    
//    if (dx * dx + dy * dy < r * r)
//    {
//      const double du = other->u - moon->u;
//      const double dv = other->v - moon->v;
//      const double alpha = (du*dx+dv*dy) / (dx*dx+dy*dy);
//      const double nu = alpha * dx;
//      const double nv = alpha * dy;
//      moon->u += nu;
//      moon->v += nv;
//      other->u -= nu;
//      other->v -= nv;
//    }
  }
}

size_t PeriodicSpatialTree::numberOfMoons() const
{
  return m_data.size();
}
