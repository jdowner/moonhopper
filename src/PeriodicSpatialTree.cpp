#include "PeriodicSpatialTree.h"
#include <cassert>
#include <cmath>
#include "Moon.h"

PeriodicSpatialTree::PeriodicSpatialTree(const PeriodicDomain& domain,
  double featureSize) :
  m_domain(domain),
  m_featureSize(featureSize)
{
  m_width = m_domain.getMaxX() - m_domain.getMinX();
  m_height = m_domain.getMaxY() - m_domain.getMinY();

  m_dx = m_width / floor(m_width / m_featureSize);
  m_dy = m_height / floor(m_height / m_featureSize);
  m_cols = static_cast<unsigned int>(m_width/m_dx);
  m_rows = static_cast<unsigned int>(m_height/m_dy);
}

void PeriodicSpatialTree::add(Moon* moon)
{
  const unsigned int hash = calculateHash(moon);
  assert(m_data.end() != m_data.find(hash));

  m_data.insert(std::make_pair(hash, moon));
}

unsigned int PeriodicSpatialTree::calculateHash(Moon* moon) const
{
  // The hash is based upon the top-left corner of the moons bounding box. This
  // reduces the number of collisions that we need to check for
  const double relX = m_domain.toX(moon->x - moon->r) - m_domain.getMinX();
  const double relY = m_domain.toY(moon->y - moon->r) - m_domain.getMinY();

  assert(relX > 0.0);
  assert(relY > 0.0);

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
    double dx = moon->x - result->second->x;
    double dy = moon->y - result->second->y;

    dx = std::min(dx, m_width - dx);
    dy = std::min(dy, m_height - dy);

    const double r = moon->r + result->second->r;

    if (dx * dx + dy * dy < r * r)
    {
      
    }
  }
}

