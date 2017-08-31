/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#include <iostream>
#include <list>
#include <string>

#include "MtlObject.hpp"

using namespace std;

int
main(int argc, char *argv[])
{
  MtlObject mtl("test.mtl");
  mtl.printMaterials();

  return 0;
}

