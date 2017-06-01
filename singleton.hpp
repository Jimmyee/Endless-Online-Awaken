#ifndef SINGLETON_HPP_INCLUDED
#define SINGLETON_HPP_INCLUDED

#include "config.hpp"
#include "eoclient.hpp"
#include "eodata.hpp"
#include "gui.hpp"
#include "map.hpp"

#include <memory>

using std::shared_ptr;

class S
{
public:
    static S& GetInstance()
    {
        static S    instance;
        return instance;
    }
private:
    S() {}
public:
    S(S const&)               = delete;
    void operator=(S const&)  = delete;

    shared_ptr<Config> config;
    shared_ptr<EOClient> eoclient;
    shared_ptr<EMF> emf;
    shared_ptr<EIF> eif;
    shared_ptr<ENF> enf;
    shared_ptr<ESF> esf;
    shared_ptr<ECF> ecf;
    shared_ptr<GUI> gui;
    shared_ptr<Map> map;
};

#endif // SINGLETON_HPP_INCLUDED
