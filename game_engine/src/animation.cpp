#include "animation.hpp"

Animation::Animation()
{
    this->current_frame = 0;
    this->frames.push_back(Frame(0));
    this->play = false;
    this->delay_count = 0;
    this->replay = false;
}

void Animation::Tick()
{
    if(this->play)
    {
        if(this->frames.size() == 0)
        {
            this->play = false;
            return;
        }

        this->delay_count++;

        if(this->delay_count >= this->frames[this->current_frame].delay)
        {
            this->delay_count = 0;
            this->current_frame++;

            if(this->current_frame >= this->frames.size())
            {
                this->play = false;

                if(this->replay)
                {
                    this->Play();
                }
            }
        }
    }
}

void Animation::Play()
{
    this->current_frame = 0;
    this->play = true;
}

void Animation::Clear()
{
    this->frames.clear();
    this->current_frame = 0;
    this->play = false;
    this->delay_count = 0;
    this->replay = false;
}
