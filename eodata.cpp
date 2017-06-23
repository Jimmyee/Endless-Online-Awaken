// Endless Online Awaken v0.0.1

/* code taken from EOServ and edited by Jimmyee */

#include "eodata.hpp"

#include "packet.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

static const char *eodata_safe_fail_filename;

static void eodata_safe_fail(int line)
{
	printf("Invalid file / failed read/seek: %s -- %i\n", eodata_safe_fail_filename, line);
	std::exit(1);
}

#define SAFE_SEEK(fh, offset, from) if (std::fseek(fh, offset, from) != 0) { std::fclose(fh); eodata_safe_fail(__LINE__); }
#define SAFE_READ(buf, size, count, fh) if (std::fread(buf, size, count, fh) != static_cast<std::size_t>(count)) { std::fclose(fh); eodata_safe_fail(__LINE__); }

EMF::EMF(int id)
{
	this->id = id;
	this->exists = false;
	this->jukebox_protect = 0.0;
	this->evacuate_lock = false;
	this->has_timed_spikes = false;

	this->Load();
}

bool EMF::Load()
{
	char namebuf[6];

	if (this->id < 0)
	{
		return false;
	}

	std::string filename = "./maps/";
	std::sprintf(namebuf, "%05i", this->id);
	filename.append(namebuf);
	filename.append(".emf");

	eodata_safe_fail_filename = filename.c_str();

	std::FILE *fh = std::fopen(filename.c_str(), "rb");

	if (!fh)
		return false;

	this->has_timed_spikes = false;

	SAFE_SEEK(fh, 0x03, SEEK_SET);
	SAFE_READ(this->rid, sizeof(char), 4, fh);

	char buf[12];
	unsigned char outersize;
	unsigned char innersize;

	SAFE_SEEK(fh, 0x1F, SEEK_SET);
	SAFE_READ(buf, sizeof(char), 2, fh);
	this->pk = PacketProcessor::Number(buf[0]) == 3;
	this->effect = static_cast<EffectType>(PacketProcessor::Number(buf[1]));

	SAFE_SEEK(fh, 0x25, SEEK_SET);
	SAFE_READ(buf, sizeof(char), 2, fh);
	this->width = PacketProcessor::Number(buf[0]) + 1;
	this->height = PacketProcessor::Number(buf[1]) + 1;

	this->tiles.resize(this->height * this->width);

	SAFE_SEEK(fh, 0x2A, SEEK_SET);
	SAFE_READ(buf, sizeof(char), 3, fh);
	this->scroll = PacketProcessor::Number(buf[0]);
	this->relog_x = PacketProcessor::Number(buf[1]);
	this->relog_y = PacketProcessor::Number(buf[2]);

	SAFE_SEEK(fh, 0x2E, SEEK_SET);
	SAFE_READ(buf, sizeof(char), 1, fh);
	outersize = PacketProcessor::Number(buf[0]);
	if (outersize)
	{
		SAFE_SEEK(fh, 8 * outersize, SEEK_CUR);
	}

	SAFE_READ(buf, sizeof(char), 1, fh);
	outersize = PacketProcessor::Number(buf[0]);
	if (outersize)
	{
		SAFE_SEEK(fh, 4 * outersize, SEEK_CUR);
	}

	SAFE_READ(buf, sizeof(char), 1, fh);
	outersize = PacketProcessor::Number(buf[0]);
	if (outersize)
	{
		SAFE_SEEK(fh, 12 * outersize, SEEK_CUR);
	}

	SAFE_READ(buf, sizeof(char), 1, fh);
	outersize = PacketProcessor::Number(buf[0]);
	for (int i = 0; i < outersize; ++i)
	{
		SAFE_READ(buf, sizeof(char), 2, fh);
		unsigned char yloc = PacketProcessor::Number(buf[0]);
		innersize = PacketProcessor::Number(buf[1]);
		for (int ii = 0; ii < innersize; ++ii)
		{
			SAFE_READ(buf, sizeof(char), 2, fh);
			unsigned char xloc = PacketProcessor::Number(buf[0]);
			unsigned char spec = PacketProcessor::Number(buf[1]);

			if (!this->InBounds(xloc, yloc))
			{
				printf("Tile spec on map %i is outside of map bounds (%ix%i)\n", this->id, xloc, yloc);
				continue;
			}

			this->GetTile(xloc, yloc).tilespec = static_cast<EMF_Tile::TileSpec>(spec);

			if (spec == (unsigned char)EMF_Tile::TileSpec::Spikes1)
			{
				this->has_timed_spikes = true;
			}
		}
	}

	SAFE_READ(buf, sizeof(char), 1, fh);
	outersize = PacketProcessor::Number(buf[0]);
	for (int i = 0; i < outersize; ++i)
	{
		SAFE_READ(buf, sizeof(char), 2, fh);
		unsigned char yloc = PacketProcessor::Number(buf[0]);
		innersize = PacketProcessor::Number(buf[1]);
		for (int ii = 0; ii < innersize; ++ii)
		{
			EMF_Warp newwarp;
			SAFE_READ(buf, sizeof(char), 8, fh);
			unsigned char xloc = PacketProcessor::Number(buf[0]);
			newwarp.map = PacketProcessor::Number(buf[1], buf[2]);
			newwarp.x = PacketProcessor::Number(buf[3]);
			newwarp.y = PacketProcessor::Number(buf[4]);
			newwarp.levelreq = PacketProcessor::Number(buf[5]);
			newwarp.spec = static_cast<EMF_Warp::WarpSpec>(PacketProcessor::Number(buf[6], buf[7]));

			if (!this->InBounds(xloc, yloc))
			{
				printf("Warp on map %i is outside of map bounds (%ix%i)\n", this->id, xloc, yloc);
				continue;
			}

			try
			{
				this->GetTile(xloc, yloc).warp = newwarp;
			}
			catch (...)
			{
				std::fclose(fh);
				eodata_safe_fail(__LINE__);
				return false;
			}
		}
	}

	SAFE_SEEK(fh, 0x2E, SEEK_SET);
	SAFE_READ(buf, sizeof(char), 1, fh);
	outersize = PacketProcessor::Number(buf[0]);
	for (int i = 0; i < outersize; ++i)
	{
		SAFE_READ(buf, sizeof(char), 8, fh);
		/*unsigned char x = */PacketProcessor::Number(buf[0]);
		/*unsigned char y = */PacketProcessor::Number(buf[1]);
		/*short npc_id = */PacketProcessor::Number(buf[2], buf[3]);
		/*unsigned char spawntype = */PacketProcessor::Number(buf[4]);
		/*short spawntime = */PacketProcessor::Number(buf[5], buf[6]);
		/*unsigned char amount = */PacketProcessor::Number(buf[7]);
	}

	SAFE_READ(buf, sizeof(char), 1, fh);
	outersize = PacketProcessor::Number(buf[0]);
	if (outersize)
	{
		SAFE_SEEK(fh, 4 * outersize, SEEK_CUR);
	}

	SAFE_READ(buf, sizeof(char), 1, fh);
	outersize = PacketProcessor::Number(buf[0]);
	for (int i = 0; i < outersize; ++i)
	{
		SAFE_READ(buf, sizeof(char), 12, fh);
		/*unsigned char x = */PacketProcessor::Number(buf[0]);
		/*unsigned char y = */PacketProcessor::Number(buf[1]);
		/*short slot = */PacketProcessor::Number(buf[4]);
		/*short itemid = */PacketProcessor::Number(buf[5], buf[6]);
		/*short time = */PacketProcessor::Number(buf[7], buf[8]);
		/*int amount = */PacketProcessor::Number(buf[9], buf[10], buf[11]);
	}

	SAFE_SEEK(fh, 0x00, SEEK_END);
	this->filesize = std::ftell(fh);

	std::fclose(fh);

	this->exists = true;

	return true;
}

void EMF::Unload()
{
	this->exists = false;

	this->tiles.clear();
}

bool EMF::InBounds(unsigned char x, unsigned char y) const
{
	return !(x >= this->width || y >= this->height);
}

bool EMF::Walkable(unsigned char x, unsigned char y, bool npc) const
{
	if (!InBounds(x, y) || !this->GetTile(x, y).Walkable(npc))
		return false;

	return true;
}

EMF_Tile& EMF::GetTile(unsigned char x, unsigned char y)
{
	if (!InBounds(x, y))
		throw std::out_of_range("Map tile out of range");

	return this->tiles[y * this->width + x];
}

const EMF_Tile& EMF::GetTile(unsigned char x, unsigned char y) const
{
	if (!InBounds(x, y))
		throw std::out_of_range("Map tile out of range");

	return this->tiles[y * this->width + x];
}

EMF_Tile::TileSpec EMF::GetSpec(unsigned char x, unsigned char y) const
{
	if (!InBounds(x, y))
		return EMF_Tile::TileSpec::None;

	return this->GetTile(x, y).tilespec;
}

EMF_Warp& EMF::GetWarp(unsigned char x, unsigned char y)
{
	return this->GetTile(x, y).warp;
}

const EMF_Warp& EMF::GetWarp(unsigned char x, unsigned char y) const
{
	return this->GetTile(x, y).warp;
}

bool EMF::Reload()
{
	char namebuf[6];
	char checkrid[4];

	std::string filename = "./maps/";
	std::sprintf(namebuf, "%05i", this->id);
	filename.append(namebuf);
	filename.append(".emf");

	std::FILE *fh = std::fopen(filename.c_str(), "rb");

	if (!fh)
	{
		printf("Could not load file: %s\n", filename.c_str());
		return false;
	}

	SAFE_SEEK(fh, 0x03, SEEK_SET);
	SAFE_READ(checkrid, sizeof(char), 4, fh);

	std::fclose(fh);

	if (this->rid[0] == checkrid[0] && this->rid[1] == checkrid[1]
	 && this->rid[2] == checkrid[2] && this->rid[3] == checkrid[3])
	{
		return true;
	}

	this->Unload();

	if (!this->Load())
	{
		return false;
	}

	this->exists = true;

	return true;
}

EMF::~EMF()
{
    this->Unload();
}

void EIF::Read(const std::string& filename)
{
	this->data.clear();

	std::FILE *fh = std::fopen(filename.c_str(), "rb");
	eodata_safe_fail_filename = filename.c_str();

	if (!fh)
	{
		printf("Could not load file: %s\n", filename.c_str());
		std::exit(1);
	}

	SAFE_SEEK(fh, 3, SEEK_SET);
	SAFE_READ(this->rid.data(), sizeof(char), 4, fh);
	SAFE_READ(this->len.data(), sizeof(char), 2, fh);
	int numobj = PacketProcessor::Number(this->len[0], this->len[1]);
	SAFE_SEEK(fh, 1, SEEK_CUR);

	unsigned char namesize;
	std::string name;
	char buf[EIF::DATA_SIZE] = {0};

	this->data.resize(numobj+1);

	SAFE_READ(static_cast<void *>(&namesize), sizeof(char), 1, fh);
	for (int i = 1; i <= numobj; ++i)
	{
		EIF_Data& newdata = this->data[i];

		namesize = PacketProcessor::Number(namesize);
		name.resize(namesize);
		SAFE_READ(&name[0], sizeof(char), namesize, fh);
		SAFE_READ(buf, sizeof(char), EIF::DATA_SIZE, fh);

		newdata.id = i;
		newdata.name = name;

		newdata.graphic = PacketProcessor::Number(buf[0], buf[1]);
		newdata.type = static_cast<EIF_Data::Type>(PacketProcessor::Number(buf[2]));
		newdata.subtype = static_cast<EIF_Data::SubType>(PacketProcessor::Number(buf[3]));
		// Ranged gun hack
		if (newdata.id == 365 && newdata.name == "Gun")
		{
			newdata.subtype = EIF_Data::SubType::Ranged;
		}
		// / Ranged gun hack
		newdata.special = static_cast<EIF_Data::Special>(PacketProcessor::Number(buf[4]));
		newdata.hp = PacketProcessor::Number(buf[5], buf[6]);
		newdata.tp = PacketProcessor::Number(buf[7], buf[8]);
		newdata.mindam = PacketProcessor::Number(buf[9], buf[10]);
		newdata.maxdam = PacketProcessor::Number(buf[11], buf[12]);
		newdata.accuracy = PacketProcessor::Number(buf[13], buf[14]);
		newdata.evade = PacketProcessor::Number(buf[15], buf[16]);
		newdata.armor = PacketProcessor::Number(buf[17], buf[18]);

		newdata.str = PacketProcessor::Number(buf[20]);
		newdata.intl = PacketProcessor::Number(buf[21]);
		newdata.wis = PacketProcessor::Number(buf[22]);
		newdata.agi = PacketProcessor::Number(buf[23]);
		newdata.con = PacketProcessor::Number(buf[24]);
		newdata.cha = PacketProcessor::Number(buf[25]);

		newdata.light = PacketProcessor::Number(buf[26]);
		newdata.dark = PacketProcessor::Number(buf[27]);
		newdata.earth = PacketProcessor::Number(buf[28]);
		newdata.air = PacketProcessor::Number(buf[29]);
		newdata.water = PacketProcessor::Number(buf[30]);
		newdata.fire = PacketProcessor::Number(buf[31]);

		newdata.scrollmap = PacketProcessor::Number(buf[32], buf[33], buf[34]);
		newdata.scrollx = PacketProcessor::Number(buf[35]);
		newdata.scrolly = PacketProcessor::Number(buf[36]);

		newdata.levelreq = PacketProcessor::Number(buf[37], buf[38]);
		newdata.classreq = PacketProcessor::Number(buf[39], buf[40]);

		newdata.strreq = PacketProcessor::Number(buf[41], buf[42]);
		newdata.intreq = PacketProcessor::Number(buf[43], buf[44]);
		newdata.wisreq = PacketProcessor::Number(buf[45], buf[46]);
		newdata.agireq = PacketProcessor::Number(buf[47], buf[48]);
		newdata.conreq = PacketProcessor::Number(buf[49], buf[50]);
		newdata.chareq = PacketProcessor::Number(buf[51], buf[52]);

		newdata.weight = PacketProcessor::Number(buf[55]);

		newdata.size = static_cast<EIF_Data::Size>(PacketProcessor::Number(buf[57]));

		if (std::fread(static_cast<void *>(&namesize), sizeof(char), 1, fh) != 1)
		{
			break;
		}
	}

	if (this->data.back().name.compare("eof") == 0)
	{
		this->data.pop_back();
	}

	printf("%i items loaded.\n", (int)this->data.size()-1);

	std::fclose(fh);
}

EIF_Data& EIF::Get(unsigned int id)
{
	if (id < this->data.size())
		return this->data[id];
	else
		return this->data[0];
}

const EIF_Data& EIF::Get(unsigned int id) const
{
	if (id < this->data.size())
		return this->data[id];
	else
		return this->data[0];
}

unsigned int EIF::GetKey(int keynum) const
{
	for (std::size_t i = 0; i < this->data.size(); ++i)
	{
		if (this->Get(i).type == EIF_Data::Type::Key && this->Get(i).key == keynum)
			return i;
	}

	return 0;
}

void ENF::Read(const std::string& filename)
{
	this->data.clear();

	std::FILE *fh = std::fopen(filename.c_str(), "rb");
	eodata_safe_fail_filename = filename.c_str();

	if (!fh)
	{
		printf("Could not load file: %s\n", filename.c_str());
		std::exit(1);
	}

	SAFE_SEEK(fh, 3, SEEK_SET);
	SAFE_READ(this->rid.data(), sizeof(char), 4, fh);
	SAFE_READ(this->len.data(), sizeof(char), 2, fh);
	int numobj = PacketProcessor::Number(this->len[0], this->len[1]);
	SAFE_SEEK(fh, 1, SEEK_CUR);

	unsigned char namesize;
	std::string name;
	char buf[ENF::DATA_SIZE] = {0};

	this->data.resize(numobj+1);

	SAFE_READ(static_cast<void *>(&namesize), sizeof(char), 1, fh);
	for (int i = 1; i <= numobj; ++i)
	{
		ENF_Data& newdata = this->data[i];

		namesize = PacketProcessor::Number(namesize);
		name.resize(namesize);
		SAFE_READ(&name[0], sizeof(char), namesize, fh);
		SAFE_READ(buf, sizeof(char), ENF::DATA_SIZE, fh);

		newdata.id = i;
		newdata.name = name;

		newdata.graphic = PacketProcessor::Number(buf[0], buf[1]);

		newdata.boss = PacketProcessor::Number(buf[3], buf[4]);
		newdata.child = PacketProcessor::Number(buf[5], buf[6]);
		newdata.type = static_cast<ENF_Data::Type>(PacketProcessor::Number(buf[7], buf[8]));
		newdata.vendor_id = PacketProcessor::Number(buf[9], buf[10]);
		newdata.hp = PacketProcessor::Number(buf[11], buf[12], buf[13]);

		newdata.mindam = PacketProcessor::Number(buf[16], buf[17]);
		newdata.maxdam = PacketProcessor::Number(buf[18], buf[19]);

		newdata.accuracy = PacketProcessor::Number(buf[20], buf[21]);
		newdata.evade = PacketProcessor::Number(buf[22], buf[23]);
		newdata.armor = PacketProcessor::Number(buf[24], buf[25]);

		newdata.exp = PacketProcessor::Number(buf[36], buf[37]);

		if (std::fread(static_cast<void *>(&namesize), sizeof(char), 1, fh) != 1)
		{
			break;
		}
	}

	if (this->data.back().name.compare("eof") == 0)
	{
		this->data.pop_back();
	}

	printf("%i npc types loaded.\n", (int)this->data.size()-1);

	std::fclose(fh);
}

ENF_Data& ENF::Get(unsigned int id)
{
	if (id < this->data.size())
		return this->data[id];
	else
		return this->data[0];
}

const ENF_Data& ENF::Get(unsigned int id) const
{
	if (id < this->data.size())
		return this->data[id];
	else
		return this->data[0];
}

void ESF::Read(const std::string& filename)
{
	this->data.clear();

	std::FILE *fh = std::fopen(filename.c_str(), "rb");
	eodata_safe_fail_filename = filename.c_str();

	if (!fh)
	{
		printf("Could not load file: %s\n", filename.c_str());
		std::exit(1);
	}

	SAFE_SEEK(fh, 3, SEEK_SET);
	SAFE_READ(this->rid.data(), sizeof(char), 4, fh);
	SAFE_READ(this->len.data(), sizeof(char), 2, fh);
	int numobj = PacketProcessor::Number(this->len[0], this->len[1]);
	SAFE_SEEK(fh, 1, SEEK_CUR);

	unsigned char namesize, shoutsize;
	std::string name, shout;
	char buf[ESF::DATA_SIZE] = {0};

	this->data.resize(numobj+1);

	SAFE_READ(static_cast<void *>(&namesize), sizeof(char), 1, fh);
	SAFE_READ(static_cast<void *>(&shoutsize), sizeof(char), 1, fh);
	for (int i = 1; i <= numobj; ++i)
	{
		ESF_Data& newdata = this->data[i];

		namesize = PacketProcessor::Number(namesize);
		name.resize(namesize);
		if (namesize > 0)
		{
			SAFE_READ(&name[0], sizeof(char), namesize, fh);
		}

		shoutsize = PacketProcessor::Number(shoutsize);
		shout.resize(shoutsize);
		if (shoutsize > 0)
		{
			SAFE_READ(&shout[0], sizeof(char), shoutsize, fh);
		}

		SAFE_READ(buf, sizeof(char), ESF::DATA_SIZE, fh);

		newdata.id = i;
		newdata.name = name;
		newdata.shout = shout;

		newdata.icon = PacketProcessor::Number(buf[0], buf[1]);
		newdata.graphic = PacketProcessor::Number(buf[2], buf[3]);
		newdata.tp = PacketProcessor::Number(buf[4], buf[5]);
		newdata.sp = PacketProcessor::Number(buf[6], buf[7]);
		newdata.cast_time = PacketProcessor::Number(buf[8]);

		newdata.type = static_cast<ESF_Data::Type>(PacketProcessor::Number(buf[11]));
		newdata.target_restrict = static_cast<ESF_Data::TargetRestrict>(PacketProcessor::Number(buf[17]));
		newdata.target = static_cast<ESF_Data::Target>(PacketProcessor::Number(buf[18]));

		newdata.mindam = PacketProcessor::Number(buf[23], buf[24]);
		newdata.maxdam = PacketProcessor::Number(buf[25], buf[26]);
		newdata.accuracy = PacketProcessor::Number(buf[27], buf[28]);
		newdata.hp = PacketProcessor::Number(buf[34], buf[35]);

		if (std::fread(static_cast<void *>(&namesize), sizeof(char), 1, fh) != 1)
		{
			break;
		}

		if (std::fread(static_cast<void *>(&shoutsize), sizeof(char), 1, fh) != 1)
		{
			break;
		}
	}

	if (this->data.back().name.compare("eof") == 0)
	{
		this->data.pop_back();
	}

	printf("%i spells loaded.\n", (int)this->data.size()-1);

	std::fclose(fh);
}

ESF_Data& ESF::Get(unsigned int id)
{
	if (id < this->data.size())
		return this->data[id];
	else
		return this->data[0];
}

const ESF_Data& ESF::Get(unsigned int id) const
{
	if (id < this->data.size())
		return this->data[id];
	else
		return this->data[0];
}

void ECF::Read(const std::string& filename)
{
	this->data.clear();

	std::FILE *fh = std::fopen(filename.c_str(), "rb");
	eodata_safe_fail_filename = filename.c_str();

	if (!fh)
	{
		printf("Could not load file: %s\n", filename.c_str());
		std::exit(1);
	}

	SAFE_SEEK(fh, 3, SEEK_SET);
	SAFE_READ(this->rid.data(), sizeof(char), 4, fh);
	SAFE_READ(this->len.data(), sizeof(char), 2, fh);
	int numobj = PacketProcessor::Number(this->len[0], this->len[1]);
	SAFE_SEEK(fh, 1, SEEK_CUR);

	unsigned char namesize;
	std::string name;
	char buf[ECF::DATA_SIZE] = {0};

	this->data.resize(numobj+1);

	SAFE_READ(static_cast<void *>(&namesize), sizeof(char), 1, fh);
	for (int i = 1; i <= numobj; ++i)
	{
		ECF_Data& newdata = this->data[i];

		namesize = PacketProcessor::Number(namesize);
		name.resize(namesize);
		SAFE_READ(&name[0], sizeof(char), namesize, fh);

		SAFE_READ(buf, sizeof(char), ECF::DATA_SIZE, fh);

		newdata.id = i;
		newdata.name = name;

		newdata.base = PacketProcessor::Number(buf[0]);
		newdata.type = PacketProcessor::Number(buf[1]);

		newdata.str = PacketProcessor::Number(buf[2], buf[3]);
		newdata.intl = PacketProcessor::Number(buf[4], buf[5]);
		newdata.wis = PacketProcessor::Number(buf[6], buf[7]);
		newdata.agi = PacketProcessor::Number(buf[8], buf[9]);
		newdata.con = PacketProcessor::Number(buf[10], buf[11]);
		newdata.cha = PacketProcessor::Number(buf[12], buf[13]);

		if (std::fread(static_cast<void *>(&namesize), sizeof(char), 1, fh) != 1)
		{
			break;
		}
	}

	if (this->data.back().name.compare("eof") == 0)
	{
		this->data.pop_back();
	}

	printf("%i classes loaded.\n", (int)this->data.size()-1);

	std::fclose(fh);
}

ECF_Data& ECF::Get(unsigned int id)
{
	if (id < this->data.size())
		return this->data[id];
	else
		return this->data[0];
}

const ECF_Data& ECF::Get(unsigned int id) const
{
	if (id < this->data.size())
		return this->data[id];
	else
		return this->data[0];
}

#undef SAFE_SEEK
#undef SAFE_READ
