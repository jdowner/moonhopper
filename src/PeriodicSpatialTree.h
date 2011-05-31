#ifndef PERIODICSPATIALTREE_H
#define PERIODICSPATIALTREE_H

#include <map>
#include <list>
#include "PeriodicDomain.h"

struct Moon;

/**
 * This class stores pointers to Moons in a binary tree according to their
 * position. It does so in such a way that collision detection is an nlog(n)
 * calculation, where n is the number of moons.
 */
class PeriodicSpatialTree
{
  public:
    PeriodicSpatialTree(const PeriodicDomain& domain, double diameter);

    void add(Moon* moon);
    size_t numberOfMoons() const;
    void clear();

    unsigned int calculateHash(Moon* moon) const;

    void resolveCollisions();
    void resolveCollisionWith(Moon* moon, unsigned int i, unsigned int j);

  private:
    typedef std::list<Moon*> MoonList;
    typedef std::map<unsigned int, Moon*> SpatialHash;

    PeriodicDomain m_domain;
    double m_diameter;
    SpatialHash m_data;
    double m_height;
    double m_width;
    double m_dx;
    double m_dy;
    unsigned int m_cols;
    unsigned int m_rows;
};

#endif // PERIODICSPATIALTREE_H

