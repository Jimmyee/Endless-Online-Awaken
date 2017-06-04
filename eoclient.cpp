#include "eoclient.hpp"
#include "singleton.hpp"

#include <string>

EOClient::EOClient(bool initialize)
{
    this->Reset();

	this->RegisterHandler(PacketFamily::PACKET_F_INIT, PacketAction::PACKET_A_INIT, INIT_INIT);

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
    this->connected = false;
}

void EOClient::Send(PacketBuilder packet)
{
    if(this->send_buffer.empty())
    {
        PacketBuilder send_packet;

        send_packet.SetID(packet.GetID());
        if(this->state != ClientState::Uninitialized)
        {
            unsigned short seqnum = static_cast<unsigned short>(this->GenSequenceByte());
            send_packet.AddShort(seqnum);
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
            this->connected = false;
            this->Reset();
        }
        else if(status == sf::Socket::Status::Error)
        {
            puts("Socket: error while receiving data");
            this->connected = false;
            this->Reset();
        }

        delete databuff;

        std::size_t datasize = 1;
        if(this->packet_state == EOClient::ReadData)
        {
            datasize = this->length;
        }

        data = this->recv_buffer.substr(0, datasize);
        this->recv_buffer.erase(0, datasize);

        while (data.length() > 0 && !done)
        {
            switch (this->packet_state)
            {
                case EOClient::ReadLen1:
                    this->raw_length[0] = data[0];
                    data[0] = '\0';
                    data.erase(0, 1);
                    this->packet_state = EOClient::ReadLen2;

                    if (data.length() == 0)
                    {
                        break;
                    }

                case EOClient::ReadLen2:
                    this->raw_length[1] = data[0];
                    data[0] = '\0';
                    data.erase(0, 1);
                    this->length = PacketProcessor::Number(this->raw_length[0], this->raw_length[1]);
                    this->packet_state = EOClient::ReadData;

                    if (data.length() == 0)
                    {
                        break;
                    }

                case EOClient::ReadData:
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
                        this->packet_state = EOClient::ReadLen1;

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
    this->packet_state = EOClient::ReadLen1;
    std::fill((std::begin((this->data))), (std::end((this->data))), '\0');
    this->data.erase();
    this->length = 0;
	this->seq_start = 0;
	this->seq = 0;
	this->session_id = 0;
	this->state = EOClient::Uninitialized;
}

void EOClient::InitSequenceByte(unsigned char s1, unsigned char s2)
{
    this->seq_start = s2 + s1 * 7 - 13;
}

void EOClient::UpdateSequenceByte(unsigned short s1, unsigned char s2)
{
    int s1int = static_cast<int>(s1);
    int s2int = static_cast<int>(s2);
    this->seq_start = s1int - s2int;
}

int EOClient::GenSequenceByte()
{
    if(++this->seq >= 10) seq = 0;

    return this->seq_start + this->seq;
}

EOClient::ClientState &EOClient::GetState()
{
    return this->state;
}

void EOClient::RequestInit()
{
    PacketBuilder builder(PacketFamily::PACKET_F_INIT, PacketAction::PACKET_A_INIT);
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
    unsigned char s1 = reader.GetByte();
    unsigned char s2 = reader.GetByte();
    unsigned char emulti_d = reader.GetByte();
    unsigned char emulti_e = reader.GetByte();
    unsigned short id = reader.GetByte();
    reader.GetThree(); // hash response

    this->processor.SetEMulti(emulti_d, emulti_e);
    this->RegisterHandler(PacketFamily::PACKET_CONNECTION, PacketAction::PACKET_PLAYER, Connection_Player);

    this->InitSequenceByte(s1, s2);
    this->session_id = id;

    this->state = EOClient::ClientState::Initialized;

    PacketBuilder packet(PacketFamily::PACKET_CONNECTION, PacketAction::PACKET_ACCEPT);
    this->Send(packet);
}
