#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include <tinyxml.h>

#include "AnimationFrame.h"

/**
 *
 */
class Animation
{
  public:
    void load(const TiXmlElement* animation);
    void play() const;
    void addFrame(const AnimationFrame& frame);

    const std::string& getSrc() const;
    unsigned int numberOfFrames() const;
    const AnimationFrame& getCurrentFrame() const;

  private:
    AnimationFrame* m_currentFrame;
    std::vector<AnimationFrame> m_frames;
    std::string m_src;
};

#endif // ANIMATION_H

