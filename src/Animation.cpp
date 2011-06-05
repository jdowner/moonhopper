#include "Animation.h"

void Animation::play() const
{
}

void Animation::addFrame(const AnimationFrame& frame)
{
  m_frames.push_back(frame);
}

