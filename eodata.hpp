
/* code taken from EOServ and edited by Jimmyee */

#ifndef EODATA_HPP_INCLUDED
#define EODATA_HPP_INCLUDED

#include "const/eodata.hpp"

#include <array>
#include <string>
#include <vector>
#include <memory>

/**
 * Object representing an item on the floor of a map
 */
struct EMF_Item
{
	short uid;
	short id;
	int amount;
	unsigned char x;
	unsigned char y;
	unsigned int owner; // Player ID
	double unprotecttime;

	EMF_Item(short uid_, short id_, int amount_, unsigned char x_, unsigned char y_, unsigned int owner_, double unprotecttime_)
	 : uid(uid_), id(id_), amount(amount_), x(x_), y(y_), owner(owner_), unprotecttime(unprotecttime_) { }
};

/**
 * Object representing a warp tile on a map, as well as storing door state
 */
struct EMF_Warp
{
	short map;
	unsigned char x;
	unsigned char y;
	unsigned char levelreq;

	enum class WarpSpec
	{
		NoDoor,
		Door,
		LockedSilver,
		LockedCrystal,
		LockedWraith
	};

	WarpSpec spec;
	bool open;

	EMF_Warp() : map(0), x(0), y(0), levelreq(0), spec(EMF_Warp::WarpSpec::NoDoor), open(false) {}

	explicit operator bool() const
	{
		return map != 0;
	}
};

/**
 * Object representing one tile on a map
 */
struct EMF_Tile
{
	enum class TileSpec
	{
		None = -1,
		Wall,
		ChairDown,
		ChairLeft,
		ChairRight,
		ChairUp,
		ChairDownRight,
		ChairUpLeft,
		ChairAll,
		Door,
		Chest,
		Unknown1,
		Unknown2,
		Unknown3,
		Unknown4,
		Unknown5,
		Unknown6,
		BankVault,
		NPCBoundary,
		MapEdge,
		FakeWall,
		Board1,
		Board2,
		Board3,
		Board4,
		Board5,
		Board6,
		Board7,
		Board8,
		Jukebox,
		Jump,
		Water,
		Unknown7,
		Arena,
		AmbientSource,
		Spikes1,
		Spikes2,
		Spikes3
	};

	TileSpec tilespec;

	EMF_Warp warp;

	EMF_Tile() : tilespec(EMF_Tile::TileSpec::None) { }

	bool Walkable(bool npc = false) const
	{
		if (this->warp && npc)
		{
			return false;
		}

		switch (this->tilespec)
		{
			case TileSpec::Wall:
			case TileSpec::ChairDown:
			case TileSpec::ChairLeft:
			case TileSpec::ChairRight:
			case TileSpec::ChairUp:
			case TileSpec::ChairDownRight:
			case TileSpec::ChairUpLeft:
			case TileSpec::ChairAll:
			case TileSpec::Chest:
			case TileSpec::BankVault:
			case TileSpec::MapEdge:
			case TileSpec::Board1:
			case TileSpec::Board2:
			case TileSpec::Board3:
			case TileSpec::Board4:
			case TileSpec::Board5:
			case TileSpec::Board6:
			case TileSpec::Board7:
			case TileSpec::Board8:
			case TileSpec::Jukebox:
				return false;
			case TileSpec::NPCBoundary:
				return !npc;
			default:
				return true;
		}
	}
};

/**
 * Contains all information about a map, holds reference to contained Characters and manages NPCs on it
 */
class EMF
{
	private:
		bool Load();
		void Unload();

	public:
		enum class EffectType
		{
			EffectNone = 0,
			EffectHPDrain = 1,
			EffectTPDrain = 2,
			EffectQuake1 = 3,
			EffectQuake2 = 4,
			EffectQuake3 = 5,
			EffectQuake4 = 6
		};

		short id;
		char rid[4];
		bool pk;
		EffectType effect;
		int filesize;
		unsigned char width;
		unsigned char height;
		bool scroll;
		unsigned char relog_x;
		unsigned char relog_y;
		std::vector<EMF_Tile> tiles;
		bool exists;
		double jukebox_protect;
		std::string jukebox_player;
		bool evacuate_lock;
		bool has_timed_spikes;

		EMF(int id);

		bool InBounds(unsigned char x, unsigned char y) const;
		bool Walkable(unsigned char x, unsigned char y, bool npc = false) const;
		EMF_Tile& GetTile(unsigned char x, unsigned char y);
		const EMF_Tile& GetTile(unsigned char x, unsigned char y) const;
		EMF_Tile::TileSpec GetSpec(unsigned char x, unsigned char y) const;
		EMF_Warp& GetWarp(unsigned char x, unsigned char y);
		const EMF_Warp& GetWarp(unsigned char x, unsigned char y) const;

		bool Reload();

		~EMF();
};

/**
 * One item record in an EIF object
 */
struct EIF_Data
{
    enum class Type
    {
        Static,
        UnknownType1,
        Money,
        Heal,
        Teleport,
        Spell,
        EXPReward,
        StatReward,
        SkillReward,
        Key,
        Weapon,
        Shield,
        Armor,
        Hat,
        Boots,
        Gloves,
        Accessory,
        Belt,
        Necklace,
        Ring,
        Armlet,
        Bracer,
        Beer,
        EffectPotion,
        HairDye,
        CureCurse
    };

    enum class SubType
    {
        None,
        Ranged,
        Arrows,
        Wings,
        TwoHanded
    };

    enum class Special
    {
        Normal,
        Rare, // ?
        UnknownSpecial2,
        Unique, // ?
        Lore,
        Cursed
    };

    enum class Size
    {
        Size1x1,
        Size1x2,
        Size1x3,
        Size1x4,
        Size2x1,
        Size2x2,
        Size2x3,
        Size2x4,
    };

	int id;
	std::string name;
	short graphic;
	Type type;
	SubType subtype;
	Special special;
	short hp;
	short tp;
	short mindam;
	short maxdam;
	short accuracy;
	short evade;
	short armor;

	unsigned char str;
	unsigned char intl;
	unsigned char wis;
	unsigned char agi;
	unsigned char con;
	unsigned char cha;

	unsigned char light;
	unsigned char dark;
	unsigned char earth;
	unsigned char air;
	unsigned char water;
	unsigned char fire;

	union
	{
		int scrollmap;
		int dollgraphic;
		int expreward;
		int haircolor;
		int effect;
		int key;
	};

	union
	{
		unsigned char gender;
		unsigned char scrollx;
	};

	union
	{
		unsigned char scrolly;
		unsigned char dual_wield_dollgraphic;
	};

	short levelreq;
	short classreq;

	short strreq;
	short intreq;
	short wisreq;
	short agireq;
	short conreq;
	short chareq;

	unsigned char weight;

	Size size;

	EIF_Data() : id(0), graphic(0), type(Type::Static), subtype(SubType::None), special(Special::Lore),
	hp(0), tp(0), mindam(0), maxdam(0), accuracy(0), evade(0), armor(0), str(0), intl(0), wis(0),
	agi(0), con(0), cha(0), light(0), dark(0), earth(0), air(0), water(0), fire(0), scrollmap(0),
	gender(0), scrolly(0), levelreq(0), classreq(0), strreq(0), intreq(0), wisreq(0), agireq(0),
	conreq(0), chareq(0), weight(0), size(Size::Size1x1) { }

	explicit operator bool() const
	{
		return id != 0;
	}
};

/**
 * Loads and stores information on all items from an EIF file
 */
class EIF
{
	public:
		static int SizeTiles(EIF_Data::Size size)
		{
			switch (size)
			{
				case EIF_Data::Size::Size1x1: return 1;
				case EIF_Data::Size::Size1x2: return 2;
				case EIF_Data::Size::Size1x3: return 3;
				case EIF_Data::Size::Size1x4: return 4;
				case EIF_Data::Size::Size2x1: return 2;
				case EIF_Data::Size::Size2x2: return 4;
				case EIF_Data::Size::Size2x3: return 6;
				case EIF_Data::Size::Size2x4: return 8;
				default:      return 0;
			}
		}

		static const int DATA_SIZE = 58;
		std::array<unsigned char, 4> rid;
		std::array<unsigned char, 2> len;
		std::vector<EIF_Data> data;

		EIF(const std::string& filename) { Read(filename.c_str()); }

		void Read(const std::string& filename);

		EIF_Data& Get(unsigned int id);
		const EIF_Data& Get(unsigned int id) const;

		unsigned int GetKey(int keynum) const;
};

/**
 * One NPC record in an ENF object
 */
struct ENF_Data
{
    enum class Type
    {
        NPC,
        Passive,
        Aggressive,
        Unknown1,
        Unknown2,
        Unknown3,
        Shop,
        Inn,
        Unknown4,
        Bank,
        Barber,
        Guild,
        Priest,
        Law,
        Skills,
        Quest
    };

	int id;
	std::string name;
	int graphic;

	short boss;
	short child;
	Type type;

	short vendor_id;

	int hp;
	unsigned short exp;
	short mindam;
	short maxdam;

	short accuracy;
	short evade;
	short armor;

	ENF_Data() : id(0), graphic(0), boss(0), child(0), type(Type::NPC), vendor_id(0),
	hp(0), exp(0), mindam(0), maxdam(0), accuracy(0), evade(0), armor(0) { }

	explicit operator bool() const
	{
		return id != 0;
	}
};

/**
 * Loads and stores information on all NPCs from an ENF file
 */
class ENF
{
	public:
		static const int DATA_SIZE = 39;
		std::array<unsigned char, 4> rid;
		std::array<unsigned char, 2> len;
		std::vector<ENF_Data> data;

		ENF(const std::string& filename) { Read(filename.c_str()); }

		void Read(const std::string& filename);

		ENF_Data& Get(unsigned int id);
		const ENF_Data& Get(unsigned int id) const;
};

/**
 * One spell record in an ESF object
 */
struct ESF_Data
{
    enum class Type
    {
        Heal,
        Damage,
        Bard
    };

    enum class TargetRestrict
    {
        NPCOnly,
        Friendly,
        Opponent
    };

    enum class Target
    {
        Normal,
        Self,
        Unknown1,
        Group
    };

	int id;
	std::string name;
	std::string shout;

	short icon;
	short graphic;

	short tp;
	short sp;

	unsigned char cast_time;

	Type type;
	TargetRestrict target_restrict;
	Target target;

	short mindam;
	short maxdam;
	short accuracy;
	short hp;

	ESF_Data() : id(0), icon(0), graphic(0), tp(0), sp(0), cast_time(0), type(Type::Damage),
	target_restrict(TargetRestrict::NPCOnly), target(Target::Normal), mindam(0), maxdam(0), accuracy(0), hp(0) { }

	explicit operator bool() const
	{
		return id != 0;
	}
};

/**
 * Loads and stores information on all spells from an ESF file
 */
class ESF
{
	public:
		static const int DATA_SIZE = 51;
		std::array<unsigned char, 4> rid;
		std::array<unsigned char, 2> len;
		std::vector<ESF_Data> data;

		ESF(const std::string& filename) { Read(filename.c_str()); }

		void Read(const std::string& filename);

		ESF_Data& Get(unsigned int id);
		const ESF_Data& Get(unsigned int id) const;
};

/**
 * One class record in an ECF object
 */
struct ECF_Data
{
	int id;
	std::string name;

	unsigned char base;
	unsigned char type;

	short str;
	short intl;
	short wis;
	short agi;
	short con;
	short cha;

	ECF_Data() : id(0), base(4), type(4), str(0), intl(0), wis(0), agi(0), con(0), cha(0) { }

	explicit operator bool() const
	{
		return id != 0;
	}
};

/**
 * Loads and stores information on all classes from an ECF file
 */
class ECF
{
	public:
		static const int DATA_SIZE = 14;
		std::array<unsigned char, 4> rid;
		std::array<unsigned char, 2> len;
		std::vector<ECF_Data> data;

		ECF(const std::string& filename) { Read(filename.c_str()); }

		void Read(const std::string& filename);

		ECF_Data& Get(unsigned int id);
		const ECF_Data& Get(unsigned int id) const;
};

#endif // EODATA_HPP_INCLUDED
