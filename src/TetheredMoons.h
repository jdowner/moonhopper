#ifndef TETHEREDMOONS_H
#define TETHEREDMOONS_H

#include "Vector2d.h"

class Moon;

/**
 * This class represents two moons that have been tethered together.
 */
class TetheredMoons
{
  public:
    TetheredMoons();
   
    bool isExtant() const;
    void reset();
    void setMoons(Moon* moonA, Moon* moonB);
    Moon* moonA();
    Moon* moonB();
    const Moon* moonA() const;
    const Moon* moonB() const;

    const Vector2d& position() const;
    const Vector2d& velocity() const;
    double angle() const;
    double angularVelocity() const;

    void moveBy(const Vector2d& delta);
    void setPosition(const Vector2d& position);
    void setPosition(double x, double y);
    void setVelocity(const Vector2d& velocity);
    void setVelocity(double u, double v);
    void setAngle(double theta);
    void setAngularVelocity(double omega);

    void applyImpulse(const Vector2d& location, const Vector2d& impulse);
    void applyImpulse(double x, double y, const Vector2d& impulse);

  private:
    void calculateInertia();
    void calculateCOM();

  private:
    Moon* m_moonA;
    Moon* m_moonB;

    Vector2d m_position;
    Vector2d m_velocity;
    double m_theta;
    double m_omega;
    double m_inertia;
    double m_totalMass;
};

#endif // TETHEREDMOONS_H

