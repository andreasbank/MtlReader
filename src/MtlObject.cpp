/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "MtlObject.hpp"

#define MATERIAL_SENINTEL "newmtl"
#define MATERIAL_SENINTEL_LEN 7

using namespace std;

MtlObject::MtlObject(const string& fileName) : mFileName(fileName)
{
  ifstream dataFile(fileName);
  string data, line;
  if (!dataFile.is_open()) {
    cerr << "Failed to open file '" << fileName << "'" << endl;
  }
  while (getline(dataFile, line))
    data += line + '\n';
  dataFile.close();
  parseMtlData(data);
}

MtlObject::~MtlObject(void)
{
  materials.clear();
}

void
MtlObject::parseMtlColorAndIllumination(const string& data,
    string::size_type& pos, MtlMaterial& mat)
{
  //string::size_type newMtl = data.find("Ka");
}

void
MtlObject::parseMtlTextureAndReflectionMaps(const string& data,
    string::size_type& pos, MtlMaterial& mat)
{
  //string::size_type mapKa = data.find("map_Ka");
  //if (mapKa) != string::npos) {
  //  MtlMap *map = new MtlMap;
  //  initializeMtlMap(*map);
  //}
}

void
MtlObject::skipOptionalChars(const string& data, string::size_type& pos)
{
  while ((data[pos] == '\'') || (data[pos] == ' ') || (data[pos] == '"'))
    ++pos;
}

void
MtlObject::parseMtlData(const string& data)
{
  string::size_type pos = 0;

  while (true) {
    pos = data.find(MATERIAL_SENINTEL, pos);
    string::size_type mtlNameEnd = data.find("\n", pos);
    if ((pos == string::npos) || (mtlNameEnd == string::npos)) {
      break;
    }
    pos += MATERIAL_SENINTEL_LEN;

    MtlMaterial *mat = new MtlMaterial();
    skipOptionalChars(data, pos);
    mat->name = data.substr(pos, mtlNameEnd - pos);
    parseMtlColorAndIllumination(data, pos, *mat);
    parseMtlTextureAndReflectionMaps(data, pos, *mat);
    materials.push_back(mat);
  }
}

void
MtlObject::printMaterials(void)
{
  cout << "Object '" << mFileName << "'" << endl;
  for(const auto mat : materials) {
    if (mat) {
      //if (is last)
      //  mat->printProperties(1, true);
      mat->printProperties(1);
    }
  }
}

