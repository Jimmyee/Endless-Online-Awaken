// Endless Online Awaken v0.0.1

#ifndef CONST_CHARACTER_HPP_INCLUDED
#define CONST_CHARACTER_HPP_INCLUDED

enum class AdminLevel : unsigned char
{
	Player = 0,
	Guide = 1,
	Guardian = 2,
	GM = 3,
	HGM = 4
};

enum class Direction : unsigned char
{
	Down = 0,
	Left = 1,
	Up = 2,
	Right = 3
};

enum class Emote : unsigned char
{
	Happy = 1,
	Depressed = 2,
	Sad = 3,
	Angry = 4,
	Confused = 5,
	Surprised = 6,
	Hearts = 7,
	Moon = 8,
	Suicidal = 9,
	Embarassed = 10,
	Drunk = 11,
	Trade = 12,
	LevelUp = 13,
	Playful = 14
};

enum class Gender : unsigned char
{
	Female = 0,
	Male = 1
};

enum class Skin : unsigned char
{
	White  = 0,
	Tan = 1,
	Pale = 2,
	Orc = 3,
	Skeleton = 4,
	Panda = 5,
	Fish = 6
};

enum class PaperdollIcon : unsigned char
{
	Normal = 0,
	GM = 4,
	HGM = 5,
	Party = 6,
	GMParty = 9,
	HGMParty = 10,
	SLNBot = 20
};

enum class AvatarSlot : unsigned char
{
	Clothes = 1,
	Hair = 2,
	HairColor = 3
};

enum class TalkReply : short
{
	NotFound = 1
};

enum class SitState : unsigned char
{
	Stand = 0,
	Chair = 1,
	Floor = 2
};

enum class SitAction : unsigned char
{
	Sit = 1,
	Stand = 2
};

enum class TrainType : unsigned char
{
    Stat = 1,
    Skill = 2
};

enum class BookIcon : short
{
    Item = 3,
    Talk = 5,
    Kill = 8,
    Step = 10
};

#endif // CONST_CHARACTER_HPP_INCLUDED
