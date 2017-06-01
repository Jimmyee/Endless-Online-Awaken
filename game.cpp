#include "game.hpp"

Game::Game()
{
    this->loaded = false;
}

void Game::Load(std::string filename)
{
    this->config.Load(filename);
    this->eoclient.Load(config);

    this->eif = shared_ptr<EIF>(new EIF("pub/dat001.eif"));
    this->enf = shared_ptr<ENF>(new ENF("pub/dtn001.eif"));
    this->esf = shared_ptr<ESF>(new ESF("pub/dsl001.eif"));
    this->ecf = shared_ptr<ECF>(new ECF("pub/dat001.eif"));
}

void Game::Tick()
{
    this->eoclient.Tick();
}

void Game::Reset()
{
    this->loaded = false;
    this->config = Config();
    this->eoclient.Reset();
}
