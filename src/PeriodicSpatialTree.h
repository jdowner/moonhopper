#ifndef PERIODICSPATIALTREE_H
#define PERIODICSPATIALTREE_H

#include <map>
#include "PeriodicDomain.h"

struct Moon;

/**
 * This class stores pointers to Moons in a binary tree according to their
 * position. It does so in such a way that collision detection is an nlog(n)
 * calculation.
 */
class PeriodicSpatialTree
{
  public:
    PeriodicSpatialTree(const PeriodicDomain& domain, double featureSize);

    void add(Moon* moon);
    void clear();

    void resolveCollisions();

  private:
    unsigned int calculateHash(Moon* moon) const;
    void resolveCollisionWith(Moon* moon, unsigned int i, unsigned int j);

  private:
    PeriodicDomain m_domain;
    double m_featureSize;
    std::map<unsigned int,Moon*> m_data;
    double m_height;
    double m_width;
    double m_dx;
    double m_dy;
    unsigned int m_cols;
    unsigned int m_rows;
};

#endif // PERIODICSPATIALTREE_H

