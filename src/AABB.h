#ifndef AABB_H
#define AABB_H

/**
 *
 */
class AABB
{
  public:
    AABB(double minx, double miny, double maxx, double maxy);

    bool contains(double x, double y) const;

  private:
    double m_minx;
    double m_miny;
    double m_maxx;
    double m_maxy;
};

#endif // AABB_H

