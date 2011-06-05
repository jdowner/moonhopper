#include "Animation.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>

namespace 
{
  template <typename T>
  T getProperty(const TiXmlElement* element, const std::string& child)
  {
    assert(element);
    assert(element->FirstChildElement(child));

    return boost::lexical_cast<T>(element->FirstChildElement(child)->GetText());
  }

  template <typename T>
  T getAttribute(const TiXmlElement* element, const std::string& attribute)
  {
    assert(element);
    assert(element->Attribute(attribute));

    return boost::lexical_cast<T>(*element->Attribute(attribute));
  }
}

void Animation::load(const TiXmlElement* animation)
{
  assert(animation);
  assert(animation->ValueStr() == "animation");

  // extract the meta information
  m_src = getAttribute<std::string>(animation, "src");

  // extract the frame information
  for(const TiXmlElement* element = animation->FirstChildElement("frame");
    element; element = element->NextSiblingElement("frame"))
  {
    AnimationFrame frame; 
    frame.height = getProperty<unsigned int>(element, "height");
    frame.width = getProperty<unsigned int>(element,"width");
    frame.period = getProperty<double>(element,"period");
    frame.offset = getProperty<unsigned int>(element,"offset");

    m_frames.push_back(frame);
  }

  // cue the current frame to the first frame in tehe sequence
  m_currentFrame = &m_frames.front();
}

void Animation::play() const
{
}

void Animation::addFrame(const AnimationFrame& frame)
{
  m_frames.push_back(frame);
}

unsigned int Animation::numberOfFrames() const
{
  return m_frames.size();
}

const AnimationFrame& Animation::getCurrentFrame() const
{
  if (m_currentFrame == NULL)
  {
    std::stringstream message;
    message << "Tried to get the current frame of an ";
    message << "but the current frame is null";
    message << std::endl;
    throw std::runtime_error(message.str());
  }

  return *m_currentFrame;
}

const std::string& Animation::getSrc() const
{
  return m_src;
}

