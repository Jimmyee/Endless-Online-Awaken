// Endless Online Awaken

#include "config.hpp"
#include "server.hpp"
#include "server_state.hpp"
#include "database.hpp"

#include <iostream>
#include <array>
#include <fstream>
#include <stdexcept>

int main()
{
    try
    {
        Server server(8078);

        std::array<int, 3> version = server.GetClientVersion();

        std::cout << "Endless Online Awaken Server v" << version[0] << "." << version[1] << "." << version[2] << std::endl
        << std::endl;

        ServerState server_state;
        Database database("data/eo.db");

        while(server_state.Get() != ServerState::State::Exit)
        {
            server.Tick();
            sf::Time time = sf::milliseconds(5);
            sf::sleep(time);
        }
    }
    catch(std::runtime_error rt_err)
    {
        std::cout << "Exception: " << rt_err.what() << std::endl;
        std::ofstream file("errorlog.txt", std::ios::trunc);
        if(file.is_open() && file.good())
        {
            file << rt_err.what() << std::endl;
            file.close();
        }
    }

    return 0;
}
