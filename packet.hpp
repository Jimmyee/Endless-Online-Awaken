
/* code taken from EOServ and edited by Jimmyee */

#ifndef PACKET_HPP_INCLUDED
#define PACKET_HPP_INCLUDED

#include <array>
#include <cstddef>
#include <string>

enum PacketFamily : unsigned char
{
	PACKET_INTERNAL = 0,
	PACKET_CONNECTION = 1,
	PACKET_ACCOUNT = 2,
	PACKET_CHARACTER = 3,
	PACKET_LOGIN = 4,
	PACKET_WELCOME = 5,
	PACKET_WALK = 6,
	PACKET_FACE = 7,
	PACKET_CHAIR = 8,
	PACKET_EMOTE = 9,
	PACKET_ATTACK = 11,
	PACKET_SPELL = 12,
	PACKET_SHOP = 13,
	PACKET_ITEM = 14,
	PACKET_STATSKILL = 16,
	PACKET_GLOBAL = 17,
	PACKET_TALK = 18,
	PACKET_WARP = 19,
	PACKET_JUKEBOX = 21,
	PACKET_PLAYERS = 22,
	PACKET_AVATAR = 23,
	PACKET_PARTY = 24,
	PACKET_REFRESH = 25,
	PACKET_NPC = 26,
	PACKET_PLAYER_AUTOREFRESH = 27,
	PACKET_NPC_AUTOREFRESH = 28,
	PACKET_APPEAR = 29,
	PACKET_PAPERDOLL = 30,
	PACKET_EFFECT = 31,
	PACKET_TRADE = 32,
	PACKET_CHEST = 33,
	PACKET_DOOR = 34,
	PACKET_MESSAGE = 35,
	PACKET_BANK = 36,
	PACKET_LOCKER = 37,
	PACKET_BARBER = 38,
	PACKET_GUILD = 39,
	PACKET_MUSIC = 40,
	PACKET_SIT = 41,
	PACKET_RECOVER = 42,
	PACKET_BOARD = 43,
	PACKET_CAST = 44,
	PACKET_ARENA = 45,
	PACKET_PRIEST = 46,
	PACKET_MARRIAGE = 47,
	PACKET_ADMININTERACT = 48,
	PACKET_CITIZEN = 49,
	PACKET_QUEST = 50,
	PACKET_BOOK = 51,
	PACKET_F_INIT = 255
};

enum PacketAction : unsigned char
{
	PACKET_REQUEST = 1,
	PACKET_ACCEPT = 2,
	PACKET_REPLY = 3,
	PACKET_REMOVE = 4,
	PACKET_AGREE = 5,
	PACKET_CREATE = 6,
	PACKET_ADD = 7,
	PACKET_PLAYER = 8,
	PACKET_TAKE = 9,
	PACKET_USE = 10,
	PACKET_BUY = 11,
	PACKET_SELL = 12,
	PACKET_OPEN = 13,
	PACKET_CLOSE = 14,
	PACKET_MSG = 15,
	PACKET_SPEC = 16,
	PACKET_ADMIN = 17,
	PACKET_LIST = 18,
	PACKET_TELL = 20,
	PACKET_REPORT = 21,
	PACKET_ANNOUNCE = 22,
	PACKET_SERVER = 23,
	PACKET_DROP = 24,
	PACKET_JUNK = 25,
	PACKET_OBTAIN = 26,
	PACKET_GET = 27,
	PACKET_KICK = 28,
	PACKET_RANK = 29,
	PACKET_TARGET_SELF = 30,
	PACKET_TARGET_OTHER = 31,
	PACKET_TARGET_GROUP = 33, // Tentative name
	PACKET_DIALOG = 34,

	PACKET_INTERNAL_NULL = 128,
	PACKET_INTERNAL_WARP = 129,

	PACKET_PING = 240,
	PACKET_PONG = 241,
	PACKET_NET3 = 242,
	PACKET_A_INIT = 255
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
		PacketBuilder(PacketFamily family = PACKET_F_INIT, PacketAction action = PACKET_A_INIT, std::size_t size_guess = 0);

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
