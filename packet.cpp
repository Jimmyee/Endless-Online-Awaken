
/* code taken from EOServ and edited by Jimmyee */

#include "packet.hpp"

#ifdef DEBUG
#endif

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>

// GCC 4.9 doesn't include these
template <class T> inline auto cbegin(T &c) -> decltype(c.cbegin()) { return c.cbegin(); }
template <class T> inline auto cbegin(const T &c) -> decltype(c.cbegin()) { return c.cbegin(); }
template <class T, size_t N> inline const T* cbegin(T(&a)[N]) { return a; }
template <class T> inline auto cend(T &c) -> decltype(c.cbegin()) { return c.cend(); }
template <class T> inline auto cend(const T &c) -> decltype(c.cbegin()) { return c.cend(); }
template <class T, size_t N> inline const T* cend(T(&a)[N]) { return a + N; }

#define UTIL_RANGE(c) (begin((c))), (end((c)))
#define UTIL_CRANGE(c) (cbegin((c))), (cend((c)))

PacketProcessor::PacketProcessor()
	: emulti_e(0)
	, emulti_d(0)
{

}

std::string PacketProcessor::GetFamilyName(PacketFamily family)
{
	switch (family)
	{
		case PacketFamily::Internal: return "Interal";
		case PacketFamily::Connection: return "Connection";
		case PacketFamily::Account: return "Account";
		case PacketFamily::Character: return "Character";
		case PacketFamily::Login: return "Login";
		case PacketFamily::Welcome: return "Welcome";
		case PacketFamily::Walk: return "Walk";
		case PacketFamily::Face: return "Face";
		case PacketFamily::Chair: return "Chair";
		case PacketFamily::Emote: return "Emote";
		case PacketFamily::Attack: return "Attack";
		case PacketFamily::Spell: return "Spell";
		case PacketFamily::Shop: return "Shop";
		case PacketFamily::Item: return "Item";
		case PacketFamily::StatSkill: return "StatSkill";
		case PacketFamily::Global: return "Global";
		case PacketFamily::Talk: return "Talk";
		case PacketFamily::Warp: return "Warp";
		case PacketFamily::Jukebox: return "Jukebox";
		case PacketFamily::Players: return "Players";
		case PacketFamily::Avatar: return "Avatar";
		case PacketFamily::Party: return "Party";
		case PacketFamily::Refresh: return "Refresh";
		case PacketFamily::NPC: return "NPC";
		case PacketFamily::PlayerAutoRefresh: return "Player_AutoRefresh";
		case PacketFamily::NPCAutoRefresh: return "NPC_AutoRefresh";
		case PacketFamily::Appear: return "Appear";
		case PacketFamily::Paperdoll: return "Paperdoll";
		case PacketFamily::Effect: return "Effect";
		case PacketFamily::Trade: return "Trade";
		case PacketFamily::Chest: return "Chest";
		case PacketFamily::Door: return "Door";
		case PacketFamily::Message: return "Message";
		case PacketFamily::Bank: return "Bank";
		case PacketFamily::Locker: return "Locker";
		case PacketFamily::Barber: return "Barber";
		case PacketFamily::Guild: return "Guild";
		case PacketFamily::Music: return "Music";
		case PacketFamily::Sit: return "Sit";
		case PacketFamily::Recover: return "Recover";
		case PacketFamily::Board: return "Board";
		case PacketFamily::Cast: return "Cast";
		case PacketFamily::Arena: return "Arena";
		case PacketFamily::Priest: return "Priest";
		case PacketFamily::Marriage: return "Marriage";
		case PacketFamily::AdminInteract: return "AdminInteract";
		case PacketFamily::Citizen: return "Citizen";
		case PacketFamily::Quest: return "Quest";
		case PacketFamily::Book: return "Book";
		case PacketFamily::Init: return "Init";
		default: return "UNKNOWN";
	}
}

std::string PacketProcessor::GetActionName(PacketAction action)
{
	switch (action)
	{
		case PacketAction::Request: return "Request";
		case PacketAction::Accept: return "Accept";
		case PacketAction::Reply: return "Reply";
		case PacketAction::Remove: return "Remove";
		case PacketAction::Agree: return "Agree";
		case PacketAction::Create: return "Create";
		case PacketAction::Add: return "Add";
		case PacketAction::Player: return "Player";
		case PacketAction::Take: return "Take";
		case PacketAction::Use: return "Use";
		case PacketAction::Buy: return "Buy";
		case PacketAction::Sell: return "Sell";
		case PacketAction::Open: return "Open";
		case PacketAction::Close: return "Close";
		case PacketAction::Msg: return "Msg";
		case PacketAction::Spec: return "Spec";
		case PacketAction::Admin: return "Admin";
		case PacketAction::List: return "List";
		case PacketAction::Tell: return "Tell";
		case PacketAction::Report: return "Report";
		case PacketAction::Announce: return "Announce";
		case PacketAction::Server: return "Server";
		case PacketAction::Drop: return "Drop";
		case PacketAction::Junk: return "Junk";
		case PacketAction::Obtain: return "Obtain";
		case PacketAction::Get: return "Get";
		case PacketAction::Kick: return "Kick";
		case PacketAction::Rank: return "Rank";
		case PacketAction::TargetSelf: return "TargetSelf";
		case PacketAction::TargetOther: return "TargetOther";
		case PacketAction::TargetGroup: return "TargetGroup";
		case PacketAction::Dialog: return "Dialog";
		case PacketAction::InternalNull: return "INTERNAL_NULL";
		case PacketAction::InternalWarp: return "INTERNAL_WARP";
		case PacketAction::Ping: return "Ping";
		case PacketAction::Pong: return "Pong";
		case PacketAction::Net3: return "Net3";
		case PacketAction::Init: return "Init";
		default: return "UNKNOWN";
	}
}

std::string PacketProcessor::Decode(const std::string &str)
{
	if (emulti_d == 0 || ((unsigned char)str[0] == (unsigned char)PacketAction::Init && (unsigned char)str[1] == (unsigned char)PacketFamily::Init))
		return str;

	std::string newstr;
	int length = str.length();
	int i = 0;
	int ii = 0;

	newstr.resize(length);

	while (i < length)
	{
		newstr[ii++] = (unsigned char)str[i] ^ 0x80;
		i += 2;
	}

	--i;

	if (length % 2)
	{
		i -= 2;
	}

	do
	{
		newstr[ii++] = (unsigned char)str[i] ^ 0x80;
		i -= 2;
	} while (i >= 0);

	for (int i = 2; i < length; ++i)
	{
		if (static_cast<unsigned char>(newstr[i]) == 128)
		{
			newstr[i] = 0;
		}
		else if (newstr[i] == 0)
		{
			newstr[i] = 128;
		}
	}

	return this->DickWinderD(newstr);
}

std::string PacketProcessor::Encode(const std::string &rawstr)
{
	if (emulti_e == 0 || ((unsigned char)rawstr[2] == (unsigned char)PacketAction::Init && (unsigned char)rawstr[3] == (unsigned char)PacketFamily::Init))
		return rawstr;

	std::string str = this->DickWinderE(rawstr);
	std::string newstr;
	int length = str.length();
	int i = 2;
	int ii = 2;

	newstr.resize(length);

	newstr[0] = str[0];
	newstr[1] = str[1];

	while (i < length)
	{
		newstr[i] = (unsigned char)str[ii++] ^ 0x80;
		i += 2;
	}

	i = length - 1;

	if (length % 2)
	{
		--i;
	}

	while (i >= 2)
	{
		newstr[i] = (unsigned char)str[ii++] ^ 0x80;
		i -= 2;
	}

	for (int i = 2; i < length; ++i)
	{
		if (static_cast<unsigned char>(newstr[i]) == 128)
		{
			newstr[i] = 0;
		}
		else if (newstr[i] == 0)
		{
			newstr[i] = 128;
		}
	}

	return newstr;
}

std::string PacketProcessor::DickWinder(const std::string &str, unsigned char emulti)
{
	std::string newstr;
	int length = str.length();
	std::string buffer;
	unsigned char c;

	if (emulti == 0)
	{
		return str;
	}

	newstr.reserve(length);

	for (int i = 0; i < length; ++i)
	{
		c = str[i];

		if (c % emulti == 0)
		{
			buffer += c;
		}
		else
		{
			if (buffer.length() > 0)
			{
				std::reverse(buffer.begin(), buffer.end());
				newstr += buffer;
				buffer.clear();
			}
			newstr += c;
		}
	}

	if (buffer.length() > 0)
	{
		std::reverse(buffer.begin(), buffer.end());
		newstr += buffer;
	}

	return newstr;
}

std::string PacketProcessor::DickWinderE(const std::string &str)
{
	return PacketProcessor::DickWinder(str, this->emulti_e);
}

std::string PacketProcessor::DickWinderD(const std::string &str)
{
	return PacketProcessor::DickWinder(str, this->emulti_d);
}

void PacketProcessor::SetEMulti(unsigned char emulti_e, unsigned char emulti_d)
{
	this->emulti_e = emulti_e;
	this->emulti_d = emulti_d;
}

unsigned int PacketProcessor::Number(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4)
{
	if (b1 == 254) b1 = 1;
	if (b2 == 254) b2 = 1;
	if (b3 == 254) b3 = 1;
	if (b4 == 254) b4 = 1;

	if (b1 == 0) b1 = 128;
	if (b2 == 0) b2 = 128;
	if (b3 == 0) b3 = 128;
	if (b4 == 0) b4 = 128;

	--b1;
	--b2;
	--b3;
	--b4;

	return (b4*PacketProcessor::MAX3 + b3*PacketProcessor::MAX2 + b2*PacketProcessor::MAX1 + b1);
}

std::array<unsigned char, 4> PacketProcessor::ENumber(unsigned int number)
{
	std::size_t throwaway;

	return PacketProcessor::ENumber(number, throwaway);
}

std::array<unsigned char, 4> PacketProcessor::ENumber(unsigned int number, std::size_t &size)
{
	std::array<unsigned char, 4> bytes{{254, 254, 254, 254}};
	unsigned int onumber = number;

	if (onumber >= PacketProcessor::MAX3)
	{
		bytes[3] = number / PacketProcessor::MAX3 + 1;
		number = number % PacketProcessor::MAX3;
	}

	if (onumber >= PacketProcessor::MAX2)
	{
		bytes[2] = number / PacketProcessor::MAX2 + 1;
		number = number % PacketProcessor::MAX2;
	}

	if (onumber >= PacketProcessor::MAX1)
	{
		bytes[1] = number / PacketProcessor::MAX1 + 1;
		number = number % PacketProcessor::MAX1;
	}

	bytes[0] = number + 1;

	for (int i = 3; i >= 0; --i)
	{
		if (i == 0)
		{
			size = 1;
			break;
		}
		else if (bytes[i] > 0)
		{
			size = i + 1;
			break;
		}
	}

	return bytes;
}

unsigned short PacketProcessor::PID(PacketFamily family, PacketAction action)
{
	return (unsigned char)family | ((unsigned char)action << 8);
}

std::array<unsigned char, 2> PacketProcessor::EPID(unsigned short pid)
{
	std::array<unsigned char, 2> b{{static_cast<unsigned char>(pid >> 8), static_cast<unsigned char>(pid & 0xFF)}};

	return b;
}

PacketReader::PacketReader(const std::string &data)
	: data(data)
	, pos(2)
{ }

std::size_t PacketReader::Length() const
{
	return this->data.length();
}

std::size_t PacketReader::Remaining() const
{
	return this->Length() - this->pos;
}

PacketAction PacketReader::Action() const
{
	if (this->Length() < 1)
		return PacketAction(0);

	return PacketAction((unsigned char)this->data[0]);
}

PacketFamily PacketReader::Family() const
{
	if (this->Length() < 2)
		return PacketFamily(0);

	return PacketFamily((unsigned char)this->data[1]);
}

unsigned int PacketReader::GetNumber(std::size_t length)
{
	std::array<unsigned char, 4> bytes{{254, 254, 254, 254}};

	std::copy_n(cbegin(this->data) + this->pos, std::min(length, this->Remaining()), begin(bytes));

	this->pos += length;

	return PacketProcessor::Number(bytes[0], bytes[1], bytes[2], bytes[3]);
}

unsigned char PacketReader::GetByte()
{
	if (this->Remaining() < 1)
		return 0;

	unsigned char ret = this->data[this->pos];
	++this->pos;

	return ret;
}

unsigned char PacketReader::GetChar()
{
	return GetNumber(1);
}

unsigned short PacketReader::GetShort()
{
	return GetNumber(2);
}

unsigned int PacketReader::GetThree()
{
	return GetNumber(3);
}

unsigned int PacketReader::GetInt()
{
	return GetNumber(4);
}

std::string PacketReader::GetFixedString(std::size_t length)
{
	if (this->Remaining() < length)
		return "";

	std::string ret = this->data.substr(this->pos, length);
	this->pos += ret.length();

	return ret;
}

std::string PacketReader::GetBreakString(unsigned char breakchar)
{
	std::string ret = GetFixedString(this->data.find_first_of(breakchar, this->pos) - this->pos);
	++this->pos;
	return ret;
}

std::string PacketReader::GetEndString()
{
	return GetFixedString(this->Remaining());
}

PacketReader::~PacketReader()
{
	std::fill(UTIL_RANGE(this->data), '\0');
}

PacketBuilder::PacketBuilder(PacketFamily family, PacketAction action, std::size_t size_guess)
	: add_size(0)
{
	this->SetID(family, action);

	this->data.reserve(size_guess);
}

unsigned short PacketBuilder::SetID(unsigned short id)
{
	if (id == 0)
	{
		id = PacketProcessor::PID(PacketFamily::Init, PacketAction::Init);
	}

	this->id = id;

	return this->id;
}

unsigned short PacketBuilder::SetID(PacketFamily family, PacketAction action)
{
	return this->SetID(PacketProcessor::PID(family,action));
}

unsigned short PacketBuilder::GetID() const
{
	return this->id;
}

std::size_t PacketBuilder::Length() const
{
	return this->data.length();
}

std::size_t PacketBuilder::Capacity() const
{
	return this->data.capacity();
}

void PacketBuilder::ReserveMore(std::size_t size_guess)
{
	size_guess += this->Length();

	if (size_guess > this->Capacity())
		this->data.reserve(size_guess);
}

#ifdef DEBUG

#define debug_packetbuilder_overflow(builder, capacity) debug_packetbuilder_overflow_(builder, capacity, __func__)

static void debug_packetbuilder_overflow_(PacketBuilder *builder, std::size_t capacity, const char *func)
{
	std::array<unsigned char, 2> id = PacketProcessor::EPID(builder->GetID());
	std::string family = PacketProcessor::GetFamilyName(PacketFamily(id[1]));
	std::string action = PacketProcessor::GetActionName(PacketAction(id[0]));
	Console::Dbg("PacketBuilder size exceeded pre-allocated capacity [%i/%i] (%s_%s via %s)", builder->Length(), capacity, family.c_str(), action.c_str(), func);
}

#endif

PacketBuilder &PacketBuilder::AddByte(unsigned char byte)
{
#ifdef DEBUG
	std::size_t capacity_before = this->Capacity();
#endif

	this->data += byte;

#ifdef DEBUG
	if (this->data.length() > capacity_before)
		debug_packetbuilder_overflow(this, capacity_before);
#endif

	return *this;
}

PacketBuilder &PacketBuilder::AddChar(unsigned char num)
{
#ifdef DEBUG
	std::size_t capacity_before = this->Capacity();
#endif

	this->data += PacketProcessor::ENumber(num)[0];

#ifdef DEBUG
	if (this->data.length() > capacity_before)
		debug_packetbuilder_overflow(this, capacity_before);
#endif

	return *this;
}

PacketBuilder &PacketBuilder::AddShort(unsigned short num)
{
#ifdef DEBUG
	std::size_t capacity_before = this->Capacity();
#endif

	this->data.append((char *)PacketProcessor::ENumber(num).data(), 2);

#ifdef DEBUG
	if (this->data.length() > capacity_before)
		debug_packetbuilder_overflow(this, capacity_before);
#endif

	return *this;
}

PacketBuilder &PacketBuilder::AddThree(unsigned int num)
{
#ifdef DEBUG
	std::size_t capacity_before = this->Capacity();
#endif

	this->data.append((char *)PacketProcessor::ENumber(num).data(), 3);

#ifdef DEBUG
	if (this->data.length() > capacity_before)
		debug_packetbuilder_overflow(this, capacity_before);
#endif

	return *this;
}

PacketBuilder &PacketBuilder::AddInt(unsigned int num)
{
#ifdef DEBUG
	std::size_t capacity_before = this->Capacity();
#endif

	this->data.append((char *)PacketProcessor::ENumber(num).data(), 4);

#ifdef DEBUG
	if (this->data.length() > capacity_before)
		debug_packetbuilder_overflow(this, capacity_before);
#endif

	return *this;
}

PacketBuilder &PacketBuilder::AddVar(int min, int max, unsigned int num)
{
	if (min <= 1 && (max <= 1 || num < PacketProcessor::MAX1))
		this->AddChar(num);
	else if (min <= 2 && (max <= 2 || num < PacketProcessor::MAX2))
		this->AddShort(num);
	else if (min <= 3 && (max <= 3 || num < PacketProcessor::MAX3))
		this->AddThree(num);
	else
		this->AddInt(num);

	return *this;
}

PacketBuilder &PacketBuilder::AddString(const std::string &str)
{
#ifdef DEBUG
	std::size_t capacity_before = this->Capacity();
#endif

	this->data += str;

#ifdef DEBUG
	if (this->data.length() > capacity_before)
		debug_packetbuilder_overflow(this, capacity_before);
#endif

	return *this;
}

PacketBuilder &PacketBuilder::AddBreakString(const std::string &str, unsigned char breakchar)
{
#ifdef DEBUG
	std::size_t capacity_before = this->Capacity();
#endif

	std::string tempstr(str);
	std::size_t breakin = tempstr.find_first_of(breakchar);

	while (breakin != std::string::npos)
	{
		tempstr[breakin] = 'y';
		breakin = tempstr.find_first_of(breakchar, breakin+1);
	}

	this->data += tempstr;
	this->data += breakchar;


#ifdef DEBUG
	if (this->data.length() > capacity_before)
		debug_packetbuilder_overflow(this, capacity_before);
#endif

	return *this;
}

void PacketBuilder::AddSize(std::size_t size)
{
	this->add_size += size;
}

void PacketBuilder::Reset(std::size_t size_guess)
{
	this->data.erase();
	this->data.reserve(size_guess);
}

std::string PacketBuilder::Get() const
{
	std::string retdata;
	retdata.reserve(4 + this->data.length());
	std::array<unsigned char, 2> id = PacketProcessor::EPID(this->id);
	std::array<unsigned char, 4> length = PacketProcessor::ENumber(this->data.length() + 2 + this->add_size);

	retdata += length[0];
	retdata += length[1];
	retdata += id[0];
	retdata += id[1];
	retdata += this->data;

	return retdata;
}

PacketBuilder::operator std::string() const
{
	return this->Get();
}

PacketBuilder::~PacketBuilder()
{
	std::fill(UTIL_RANGE(this->data), '\0');
}
