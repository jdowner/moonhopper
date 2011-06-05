#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

#include "AnimationFrame.h"

/**
 *
 */
class Animation
{
  public:
    void play() const;
    void addFrame(const AnimationFrame& frame);

  private:
    std::vector<AnimationFrame> m_frames;
};

#endif // ANIMATION_H

