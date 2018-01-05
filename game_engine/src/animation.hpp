#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

#include <vector>

class Animation
{
public:
    struct Frame
    {
        int delay;

        Frame() : delay(0) { }
        Frame(int delay) : delay(delay) { }
    };

public:
    std::vector<Frame> frames;
    std::size_t current_frame;
    bool play;
    int delay_count;
    bool replay;

    Animation();
    void Tick();
    void Play();
    void Clear();
};

#endif // ANIMATION_HPP_INCLUDED
