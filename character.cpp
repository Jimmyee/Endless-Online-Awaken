// Endless Online Awaken v0.0.1

#include "character.hpp"
#include "singleton.hpp"

Animation::Animation()
{
    this->frames = 1;
    this->delay = 100;
    this->current_frame = 0;
    this->play = false;
}

Animation::Animation(int frames, int delay)
{
    this->frames = frames;
    this->delay = delay;
    this->current_frame = 0;
    this->play = false;

    this->clock.restart();
}

int Animation::Process()
{
    if(this->clock.getElapsedTime().asMilliseconds() >= this->delay / this->frames)
    {
        if(this->play)
        {
            puts("PLAY");
            if(++this->current_frame >= frames)
            {
                this->current_frame = 0;
                this->play = false;
            }
            else
            {
                this->clock.restart();
            }

            printf("Frame: %i\n", this->current_frame);
        }
    }

    return this->current_frame;
}

void Animation::Play()
{
    if(!this->play)
    {
        this->play = true;
        this->current_frame = 0;

        //this->clock.restart();
    }
}

Character::Character()
{
    this->id = 0;
    this->gameworld_id = 0;
    this->clas = 0;
    this->admin_level = static_cast<AdminLevel>(0);
    this->level = 0;
    this->exp = 0;
    this->usage = 0;
    this->hp = 0;
    this->max_hp = 0;
    this->tp = 0;
    this->max_tp = 0;
    this->max_sp = 0;
    this->stat_points = 0;
    this->skill_points = 0;
    this->karma = 0;
    this->min_dam = 0;
    this->max_dam = 0;
    this->accuracy = 0;
    this->evade = 0;
    this->armor = 0;
    this->wis = 0;
    this->intl = 0;
    this->agi = 0;
    this->con = 0;
    this->cha = 0;

    this->weight = 0;
    this->max_weight = 0;

    this->x = 0;
    this->y = 0;
    this->direction = static_cast<Direction>(0);
    this->gender = static_cast<Gender>(0);
    this->hair_style = 0;
    this->hair_color = 0;
    this->race = static_cast<Skin>(0);

    this->sitting = static_cast<SitState>(0);
    this->visibility = 0;

    this->animation = Animation(4, 460);
    this->xoff = 0;
    this->yoff = 0;
}

void Character::Process()
{
    int frame = this->animation.Process();

    frame++;

    if(this->animation.play)
    {
        this->xoff = (5 - frame) * 8;
        this->yoff = (5 - frame) * 4;
    }
    else
    {
        this->xoff = 0;
        this->yoff = 0;
    }
}

void Character::Draw(int x, int y)
{
    S &s = S::GetInstance();

    shared_ptr<sf::Texture> texture = s.gfx_loader.LoadTexture(8, 1);
    sf::IntRect rect(0, 0, 18, 59);
    sf::Sprite sprite(*texture, rect);
    sprite.setPosition(sf::Vector2f(x + 32 - 9, y - 59 + 24));

    s.window.draw(sprite);
}
