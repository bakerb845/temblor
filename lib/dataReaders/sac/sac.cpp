#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include "temblor/library/dataReaders/sac/sac.hpp"


using namespace Temblor::Library::DataReaders::SAC;

class SAC::SACImpl
{
public:
    SACImpl() = default;
    
        
    class Header header;

};

SAC::SAC() :
    pImpl(std::make_unique<SACImpl> ())
{
}

SAC::~SAC() = default;

