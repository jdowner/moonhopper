#ifndef PERIODICDOMAIN_H
#define PERIODICDOMAIN_H

/**
 * A periodic domain is a rectangular region where the edges wrap around, i.e.
 * something passing out through the right side would enter from the left. It is
 * essentially a torus.
 */
class PeriodicDomain
{
  public:
    PeriodicDomain(double minx, double maxx, double miny, double maxy);

    double toX(double x) const;
    double toY(double y) const;

    double getMinX() const;
    double getMaxX() const;
    double getMinY() const;
    double getMaxY() const;

  private:
    double m_minx;
    double m_maxx;
    double m_miny;
    double m_maxy;
    double m_diffx;
    double m_diffy;
};

#endif // PERIODICDOMAIN_H

