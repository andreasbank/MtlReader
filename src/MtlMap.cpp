/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#include "MtlMap.hpp"

using namespace std;

MtlMap::MtlMap(const string& mapName) : name(mapName)
{}

void
MtlMap::printProperties(const string& prefix, bool isLast)
{
  cout << prefix << (isLast ? " └─" : " ├─") << "Map name: " << name << endl;
}
