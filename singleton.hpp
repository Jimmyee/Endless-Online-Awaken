#ifndef SINGLETON_HPP_INCLUDED
#define SINGLETON_HPP_INCLUDED

class S
{
public:
    static S& getInstance()
    {
        static S    instance;
        return instance;
    }
private:
    S() {}
    S(S const&);
    void operator=(S const&);
public:
    S(S const&)               = delete;
    void operator=(S const&)  = delete;


};

#endif // SINGLETON_HPP_INCLUDED
