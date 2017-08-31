/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#include "MtlMap.hpp"

using namespace std;

MtlMap::MtlMap(const string& mapName) : name(mapName)
{}

void
MtlMap::printProperties(int tableLevel, bool isLast)
{
  string spaces;

  for (int i = 0; i < tableLevel; ++i) {
    spaces += " ";
  }

  cout << spaces << (isLast ? "└─" : "├─") << "Map name: " << name << endl;
  spaces += (isLast ? " " : "│") + string(" ├─");
}
