#ifndef COLLISIONRESOLUTION_H
#define COLLISIONRESOLUTION_H

class Moon;
class PeriodicDomain;

void elasticCollision(const PeriodicDomain& domain, Moon& moonA, Moon& moonB);

#endif // COLLISIONRESOLUTION_H

