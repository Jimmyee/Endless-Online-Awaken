#ifndef FWD_INIT_HPP_INCLUDED
#define FWD_INIT_HPP_INCLUDED

enum class InitReply : unsigned char
{
	OutOfDate = 1,
	OK = 2,
	Banned = 3,
	Map = 4,
	EIF = 5,
	ENF = 6,
	ESF = 7,
	Players = 8,
	MapMutation = 9,
	FriendListPlayers = 10,
	ECF = 11
};

enum class InitBanType : unsigned char
{
	BanTemporary = 0,
	BanPermanent = 2
};

#endif // FWD_INIT_HPP_INCLUDED
