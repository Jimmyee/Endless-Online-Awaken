
/* code taken from EOServ and edited by Jimmyee */

#ifndef PACKET_HPP_INCLUDED
#define PACKET_HPP_INCLUDED

#include <array>
#include <cstddef>
#include <string>

enum class PacketFamily : unsigned char
{
	Internal = 0,
	Connection = 1,
	Account = 2,
	Character = 3,
	Login = 4,
	Welcome = 5,
	Walk = 6,
	Face = 7,
	Chair = 8,
	Emote = 9,
	Attack = 11,
	Spell = 12,
	Shop = 13,
	Item = 14,
	StatSkill = 16,
	Global = 17,
	Talk = 18,
	Warp = 19,
	Jukebox = 21,
	Players = 22,
	Avatar = 23,
	Party = 24,
	Refresh = 25,
	NPC = 26,
	PlayerAutoRefresh = 27,
	NPCAutoRefresh = 28,
	Appear = 29,
	Paperdoll = 30,
	Effect = 31,
	Trade = 32,
	Chest = 33,
	Door = 34,
	Message = 35,
	Bank = 36,
	Locker = 37,
	Barber = 38,
	Guild = 39,
	Music = 40,
	Sit = 41,
	Recover = 42,
	Board = 43,
	Cast = 44,
	Arena = 45,
	Priest = 46,
	Marriage = 47,
	AdminInteract = 48,
	Citizen = 49,
	Quest = 50,
	Book = 51,
	Init = 255
};

enum PacketAction : unsigned char
{
	Request = 1,
	Accept = 2,
	Reply = 3,
	Remove = 4,
	Agree = 5,
	Create = 6,
	Add = 7,
	Player = 8,
	Take = 9,
	Use = 10,
	Buy = 11,
	Sell = 12,
	Open = 13,
	Close = 14,
	Msg = 15,
	Spec = 16,
	Admin = 17,
	List = 18,
	Tell = 20,
	Report = 21,
	Announce = 22,
	Server = 23,
	Drop = 24,
	Junk = 25,
	Obtain = 26,
	Get = 27,
	Kick = 28,
	Rank = 29,
	TargetSelf = 30,
	TargetOther = 31,
	TargetGroup = 33, // Tentative name
	Dialog = 34,

	InternalNull = 128,
	InternalWarp = 129,

	Ping = 240,
	Pong = 241,
	Net3 = 242,
	Init = 255
};

/**
 * Encodes and Decodes packets for a Client.
 * Each Client needs an instance of this because it holds connection-specific data required to function correctly.
 */
class PacketProcessor
{
	protected:
		/**
		 * "EMulti" variable for Encoding.
		 */
		unsigned char emulti_e;

		/**
		 * "EMulti" variable for Decoding.
		 */
		unsigned char emulti_d;

	public:
		/**
		 * Highest number EO can represent with 1 byte.
		 */
		static const unsigned int MAX1 = 253;

		/**
		 * Highest number EO can represent with 2 bytes.
		 */
		static const unsigned int MAX2 = 64009;

		/**
		 * Highest number EO can represent with 3 bytes.
		 */
		static const unsigned int MAX3 = 16194277;

		PacketProcessor();

		/**
		 * Return a string describing a packet's family ID.
		 */
		static std::string GetFamilyName(PacketFamily family);

		/**
		 * Return a string describing a packet's action ID.
		 */
		static std::string GetActionName(PacketAction action);

		std::string Decode(const std::string &);
		std::string Encode(const std::string &);
		static std::string DickWinder(const std::string &, unsigned char emulti);
		std::string DickWinderE(const std::string &);
		std::string DickWinderD(const std::string &);

		void SetEMulti(unsigned char, unsigned char);
		static std::array<unsigned char, 2> GetEMulti();

		static unsigned int Number(unsigned char, unsigned char = 254, unsigned char = 254, unsigned char = 254);
		static std::array<unsigned char, 4> ENumber(unsigned int);
		static std::array<unsigned char, 4> ENumber(unsigned int, std::size_t &size);

		static unsigned short PID(PacketFamily family, PacketAction action);
		static std::array<unsigned char, 2> EPID(unsigned short id);
};

class PacketReader
{
	protected:
		std::string data;
		std::size_t pos;

	public:
		PacketReader(const std::string &);

		std::size_t Length() const;
		std::size_t Remaining() const;

		PacketAction Action() const;
		PacketFamily Family() const;

		unsigned int GetNumber(std::size_t length);

		unsigned char GetByte();
		unsigned char GetChar();
		unsigned short GetShort();
		unsigned int GetThree();
		unsigned int GetInt();

		std::string GetFixedString(std::size_t length);
		std::string GetBreakString(unsigned char breakchar = 0xFF);
		std::string GetEndString();

		~PacketReader();
};

class PacketBuilder
{
	protected:
		unsigned short id;
		std::string data;
		std::size_t add_size;

	public:
		PacketBuilder(PacketFamily family = PacketFamily::Init, PacketAction action = PacketAction::Init, std::size_t size_guess = 0);

		unsigned short SetID(unsigned short id);
		unsigned short SetID(PacketFamily family, PacketAction action);

		unsigned short GetID() const;

		std::size_t Length() const;
		std::size_t Capacity() const;

		void ReserveMore(std::size_t size_guess);

		PacketBuilder &AddByte(unsigned char);
		PacketBuilder &AddChar(unsigned char);
		PacketBuilder &AddShort(unsigned short);
		PacketBuilder &AddThree(unsigned int);
		PacketBuilder &AddInt(unsigned int);
		PacketBuilder &AddVar(int min, int max, unsigned int);

		PacketBuilder &AddString(const std::string &);
		PacketBuilder &AddBreakString(const std::string &, unsigned char breakchar = 0xFF);

		void AddSize(std::size_t size);

		void Reset(std::size_t size_guess = 0);

		std::string Get() const;

		operator std::string() const;

		~PacketBuilder();
};

#endif // PACKET_HPP_INCLUDED
