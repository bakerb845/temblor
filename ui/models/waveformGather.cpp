#include <cstdio>
#include <cstdlib>
#include "temblor/userInterface/models/waveformGather.hpp"

using namespace Temblor::UserInterface::Models;

class WaveformGather::WaveformGatherImpl
{
public:
};

WaveformGather::WaveformGather() :
    pImpl(std::make_unique<WaveformGatherImpl> ())
{
}

WaveformGather::~WaveformGather() = default;

