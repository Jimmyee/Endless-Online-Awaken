#include "eoclient.hpp"
#include "singleton.hpp"

#include <string>

EOClient::EOClient(bool initialize)
{
    this->Reset();

	this->RegisterHandler(PacketFamily::Init, PacketAction::Init, INIT_INIT);

	if(initialize)
    {
        if(this->Connect())
        {
            this->RequestInit();
        }
    }
}

bool EOClient::Connect()
{
    shared_ptr<Config> config = S::GetInstance().config;

    std::string address = config->values["Address"];
    unsigned short port = std::atoi(config->values["Port"].c_str());

    printf("Socket: Connecting to %s:%i\n", address.c_str(), port);
    sf::Socket::Status status = this->socket.connect(address, port);

    if(status != sf::Socket::Done)
    {
        puts("Socket: couldn't connect.");
        return false;
    }

    this->socket.setBlocking(false);
    this->connected = true;
    puts("Socket: Connected");

    return true;
}

void EOClient::Disconnect()
{
    this->socket.disconnect();
    this->Reset();
}

bool EOClient::Connected()
{
    return this->connected;
}

void EOClient::Send(PacketBuilder packet)
{
    if(this->send_buffer.empty())
    {
        PacketBuilder send_packet;

        send_packet.SetID(packet.GetID());
        if(this->state != State::Uninitialized)
        {
            int seq_byte = this->GenSequenceByte();
            if(seq_byte >= 253)
            {
                unsigned short seq_byte_short = seq_byte;
                printf("Seq byte: %i\n", seq_byte_short);
                send_packet.AddShort(seq_byte);
            }
            else
            {
                send_packet.AddChar(seq_byte);
            }
        }
        send_packet.AddString(packet.Get().substr(4));
        std::string enc = this->processor.Encode(send_packet.Get());
        const char *data = enc.c_str();
        std::size_t sent = 0;

        sf::Socket::Status status = this->socket.send(data, enc.size(), sent);
        if(status == sf::Socket::Done)
        {

        }
        else if(status == sf::Socket::Partial)
        {
            this->send_buffer = enc.substr(sent);
        }
        else
        {
            puts("Socket: couldn't send data");
        }
    }
}

void EOClient::HandleData(std::string data)
{
    PacketReader reader(this->processor.Decode(data));
    std::map<PacketFamily, std::map<PacketAction, handler_func>>::iterator it;

    it = this->handlers.find(reader.Family());
    if(it != this->handlers.end())
    {
        this->handlers[reader.Family()][reader.Action()](reader);
    }
}

void EOClient::RegisterHandler(PacketFamily family, PacketAction action, handler_func func)
{
    this->handlers[family][action] = func;
}

void EOClient::UnregisterHandler(PacketFamily family, PacketAction action)
{
    std::map<PacketFamily, std::map<PacketAction, handler_func>>::iterator it;
    it = this->handlers.find(family);
    if(it != this->handlers.end())
    {
        this->handlers.erase(it);
    }
}

void EOClient::Tick()
{
    if(this->connected)
    {
        char *databuff = new char[1024];
        std::size_t received = 0;

        std::string data;
        bool done = false;
        int oldlength;

        sf::Socket::Status status = this->socket.receive(databuff, 1024, received);

        if(status == sf::Socket::Status::Done)
        {
            std::string datastr(databuff);
            this->recv_buffer += datastr.substr(0, received);
        }
        else if(status == sf::Socket::Status::NotReady)
        {

        }
        else if(status == sf::Socket::Status::Disconnected)
        {
            puts("Socket: disconnected");
            this->Reset();
        }
        else if(status == sf::Socket::Status::Error)
        {
            puts("Socket: error while receiving data");
            this->Reset();
        }

        delete databuff;

        std::size_t datasize = 1;
        if(this->packet_state == EOClient::PacketState::ReadData)
        {
            datasize = this->length;
        }

        data = this->recv_buffer.substr(0, datasize);
        std::fill(this->recv_buffer.begin(), this->recv_buffer.begin() + datasize, '\0');
        this->recv_buffer.erase(0, datasize);

        while (data.length() > 0 && !done)
        {
            switch (this->packet_state)
            {
                case EOClient::PacketState::ReadLen1:
                    this->raw_length[0] = data[0];
                    data[0] = '\0';
                    data.erase(0, 1);
                    this->packet_state = EOClient::PacketState::ReadLen2;

                    if (data.length() == 0)
                    {
                        break;
                    }

                case EOClient::PacketState::ReadLen2:
                    this->raw_length[1] = data[0];
                    data[0] = '\0';
                    data.erase(0, 1);
                    this->length = PacketProcessor::Number(this->raw_length[0], this->raw_length[1]);
                    this->packet_state = EOClient::PacketState::ReadData;

                    if (data.length() == 0)
                    {
                        break;
                    }

                case EOClient::PacketState::ReadData:
                    oldlength = this->data.length();
                    this->data += data.substr(0, this->length);
                    std::fill(data.begin(), data.begin() + std::min<std::size_t>(data.length(), this->length), '\0');
                    data.erase(0, this->length);
                    this->length -= this->data.length() - oldlength;

                    if (this->length == 0)
                    {
                        PacketReader reader(this->processor.Decode(this->data));

                        printf("[Server] %s:%s\n",
                               this->processor.GetFamilyName(reader.Family()).c_str(),
                               this->processor.GetActionName(reader.Action()).c_str());
                        this->HandleData(this->data);

                        std::fill((std::begin((this->data))), (std::end((this->data))), '\0');
                        this->data.erase();
                        this->packet_state = EOClient::PacketState::ReadLen1;

                        done = true;
                    }
                    break;

                default:
                    // If the code ever gets here, something is broken, so we just reset the client's state.
                    // thanks Sausage
                    std::fill((std::begin((data))), (std::end((data))), '\0');
                    data.erase();
                    this->Reset();
            }
        }

        if(!this->send_buffer.empty())
        {
            const char *to_send = this->send_buffer.c_str();
            std::size_t sent = 0;

            sf::Socket::Status status = this->socket.send(to_send, this->send_buffer.size(), sent);
            if(status == sf::Socket::Done)
            {
                this->send_buffer.clear();
            }
            else if(status == sf::Socket::Partial)
            {
                this->send_buffer.erase(sent);
            }
        }
    }
}

void EOClient::Reset()
{
    this->connected = false;
    std::fill((std::begin((this->send_buffer))), (std::end((this->send_buffer))), '\0');
    this->send_buffer.erase();
    std::fill((std::begin((this->recv_buffer))), (std::end((this->recv_buffer))), '\0');
    this->recv_buffer.erase();
    this->packet_state = EOClient::PacketState::ReadLen1;
    std::fill((std::begin((this->data))), (std::end((this->data))), '\0');
    this->data.erase();
    this->length = 0;
	this->seq_start = 0;
	this->seq = 0;
	this->session_id = 0;
	this->state = EOClient::State::Uninitialized;
}

void EOClient::InitSequenceByte(unsigned char s1, unsigned char s2)
{
	this->seq_start = std::uint32_t(s2 + s1 * 7);
	this->seq_start -= 13;
}

void EOClient::UpdateSequenceByte(unsigned short s1, unsigned char s2)
{
    this->seq_start = std::uint32_t(s1 - s2);

    printf("s1: %i, s2: %i, seq_start: %i, seq: %i\n", s1, s2, this->seq_start, this->seq);
}

int EOClient::GenSequenceByte()
{
    if(++this->seq >= 10) seq = 0;

    int ret = std::uint32_t(this->seq_start + this->seq);

    printf("seq_start: %i, seq: %i\n", this->seq_start, this->seq);

    return ret;
}

void EOClient::SetState(State state)
{
    this->state = state;
}

EOClient::State EOClient::GetState()
{
    return this->state;
}

void EOClient::RequestInit()
{
    PacketBuilder builder(PacketFamily::Init, PacketAction::Init);
    unsigned int challenge = 72000;
    builder.AddThree(challenge);
    builder.AddChar(2); // ?
    builder.AddChar(2); // ?
    builder.AddChar(28); // version
    builder.AddChar(2); // ?
    builder.AddChar(2); // ?
    builder.AddString("56781234567"); // HDD ID

    this->Send(builder);
}

void EOClient::Initialize(PacketReader reader)
{
    int s1 = reader.GetByte();
    int s2 = reader.GetByte();
    int emulti_d = reader.GetByte();
    int emulti_e = reader.GetByte();
    int id = reader.GetShort();
    reader.GetThree(); // hash response

    this->processor.SetEMulti(emulti_e, emulti_d);

    this->InitSequenceByte(s1, s2);
    this->session_id = id;

    this->state = EOClient::State::Initialized;
}

void EOClient::LoginRequest(std::string username, std::string password)
{
    PacketBuilder packet(PacketFamily::Login, PacketAction::Request);
    packet.AddBreakString(username);
    packet.AddBreakString(password);
    this->Send(packet);
}

void EOClient::AccountRequest(std::string username)
{
    PacketBuilder packet(PacketFamily::Account, PacketAction::Request);
    packet.AddString(username);
    this->Send(packet);
}

void EOClient::AccountCreate(std::string username, std::string password, std::string real_name, std::string location, std::string email)
{
    std::string computer = "Endless Online Awaken";

	PacketBuilder packet(PacketFamily::Account, PacketAction::Create);
	packet.AddShort(this->session_id);
	packet.AddByte(1); // ?
	packet.AddBreakString(username);
	packet.AddBreakString(password);
	packet.AddBreakString(real_name);
	packet.AddBreakString(location);
	packet.AddBreakString(email);
	packet.AddBreakString(computer);
	packet.AddBreakString("56781234567"); // HDD ID
	this->Send(packet);
}

void EOClient::RequestSelectCharacter(unsigned int id)
{
    PacketBuilder packet(PacketFamily::Welcome, PacketAction::Request);
    packet.AddInt(id);
    this->Send(packet);
}

shared_ptr<Character> EOClient::GetAccountCharacter(std::size_t index)
{
    for(std::size_t i = 0; i < this->account_characters.size(); ++i)
    {
        if(i == index)
        {
            return this->account_characters[i];
        }
    }

    return shared_ptr<Character>(0);
}
