#!/usr/bin/env python3
import datetime
ifl = open('leap-seconds.txt', 'r')
cdat = ifl.read()
ifl.close()
cdat = cdat.split('\n')
tepoch = datetime.datetime(1970, 1, 1, 0, 0, 0,
                               tzinfo=datetime.timezone.utc)
t1900  = datetime.datetime(1900, 1, 1, 0, 0, 0,
                           tzinfo=datetime.timezone.utc)
tshift = tepoch - t1900
tremove = int(tshift.total_seconds())
#print(tremove)

k = 0
cout = '// THIS IS COMPUTER GENERATED CODE - MODIFICATION IS FUTILE\n' \
     + '#include <cstdio>\n' \
     + '#include <cstdlib>\n' \
     + '#include <vector>\n' \
     + '#include "temblor/utilities/leapSeconds.hpp"\n' \
     + 'using namespace Temblor::Utilities;\n' \
     + 'class LeapSeconds::LeapSecondsImpl\n' \
     + '{\n' \
     + 'public:\n'

et = []
nls = []
for i in range(len(cdat)-1):
    cline = cdat[i]
    if (cline[0] == '#'):
        continue
    csplit = cline.split('\t')
    time1900 = int(csplit[0])
    leapsecond = int(csplit[1])
    #print(k, time1900 - tremove, ',', leapsecond)
    t1969 = time1900 - tremove
    #if (k == 0):
    #    assert t1969 - 63072000 == 0, 'Time conversion failed'
    #cout = cout \
    #     + "               {%d,%d},\n"%(t1969, leapsecond)
    et.append(t1969)
    nls.append(leapsecond)
    k = k + 1

cout = cout + '     std::vector<int64_t> mEpoch = {\n'
for i in range(len(et)):
    if (i == 0):
        assert et[i] - 63072000 == 0, 'Time conversion failed'
    cout = cout + '           %d'%(et[i])
    if (i < len(et) - 1):
        cout = cout + ',\n' 
    else:
        cout = cout + '};\n'
cout = cout + '     std::vector<int> mLeapSeconds = {\n'
for i in range(len(nls)):
    cout = cout + '           %d'%(nls[i])
    if (i < len(nls) - 1):
        cout = cout + ',\n'
    else:
        cout = cout + '};\n' 
cout = cout + '};\n'
# Now the rest of the code
cout = cout + "/// Constructor\n" \
     + "LeapSeconds::LeapSeconds() :\n" \
     + "pImpl(std::make_unique<LeapSecondsImpl> ())\n" \
     + "{\n" \
     + "}\n" \
     + "/// Copy constructor\n" \
     + "LeapSeconds::LeapSeconds(const LeapSeconds &leapSeconds)\n" \
     + "{\n" \
     + "    *this = leapSeconds;\n" \
     + "}\n" \
     + "/// Move constructor\n" \
     + "LeapSeconds::LeapSeconds(LeapSeconds &&leapSeconds) noexcept\n" \
     + "{\n" \
     + "    *this = std::move(leapSeconds);\n" \
     + "}\n" \
     + "/// Copy assignment operators\n" \
     + "LeapSeconds& LeapSeconds::operator=(const LeapSeconds &leapSeconds)\n" \
     + "{\n" \
     + "    if (&leapSeconds == this){return *this;}\n" \
     + "    pImpl = std::make_unique<LeapSecondsImpl> (*leapSeconds.pImpl);\n" \
     + "    return *this;\n" \
     + "}\n" \
     + "/// Move assignment operators\n" \
     + "LeapSeconds& LeapSeconds::operator=(LeapSeconds &&leapSeconds) noexcept\n" \
     + "{\n" \
     + "    if (&leapSeconds == this){return *this;}\n" \
     + "    pImpl = std::move(leapSeconds.pImpl);\n" \
     + "    return *this;\n" \
     + "}\n" \
     + "/// Destructor\n" \
     + "LeapSeconds::~LeapSeconds() = default;\n" \
     + "/// Get number of leapseconds\n" \
     + "int LeapSeconds::getNumberOfLeapSeconds(const double epoch) const\n" \
     + "{\n" \
     + "    auto t = static_cast<int64_t> (epoch);\n" \
     + "    // More often than not we're working in the present\n" \
     + "    if (t >= pImpl->mEpoch.back()){return pImpl->mLeapSeconds.back();}\n" \
     + "    // Fix edge case\n" \
     + "    if (t <= pImpl->mEpoch.front()){return pImpl->mLeapSeconds.front();}\n" \
     + "    auto low = std::lower_bound(pImpl->mEpoch.begin(), pImpl->mEpoch.end(), t);\n" \
     + "    // lower_bound tells us where to insert - so back up 1\n" \
     + "    if (*low != t){low = low - 1;}\n" \
     + "    auto index = std::distance(pImpl->mEpoch.begin(), low);\n" \
     + "    return pImpl->mLeapSeconds[index];\n" \
     + "}\n"

ofl = open('leapSeconds.cpp', 'w')
ofl.write(cout)
ofl.close()
#print(cout) 
