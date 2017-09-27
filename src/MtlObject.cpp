/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "MtlObject.hpp"

#define MATERIAL_SENINTEL "newmtl"
#define MATERIAL_SENINTEL_LEN 7

using namespace std;

class MtlParseException : public exception {
   const char * what () const throw () {
      return "Failed parsing the file";
   }
};

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
MtlObject::skipOptionalChars(const string& data, string::size_type& pos)
{
  while ((data[pos] == '\'') || (data[pos] == ' ') || (data[pos] == '"'))
    ++pos;
}

float
MtlObject::parseNextFloat(const string& data, string::size_type& pos)
{
  float f = 0.0f;
  string::size_type localPos = pos;
  skipOptionalChars(data, localPos);
  if ((localPos = data.find("Ka", localPos)) != string::npos) {
    cout << "Found Ka" << endl;
    localPos += 2;
    string::size_type endPos = data.find(" ", localPos);
    string floatString = data.substr(localPos, endPos);
    f = stof(floatString);
  }

  return f;
}

void
MtlObject::parseMtlColorAndIllumination(const string& data,
    string::size_type& pos, MtlMaterial& mat)
{
  /*
  MtlColor ambientColor, diffuseColor, specularColor; // Ka, Kd, Ks (3 * [0 - 255])
  int illumination; // illum
  float dissolve; // d (0.0 - 1.0)
  float specularExponent; // Ns (0 - 1000)
  float sharpness; // sharpness
  float opticalDensity; // Ni (0.001 - 10.0)
  MtlMap mapAmbientColor; // map_Ka (options filename)
  MtlMap mapDiffuseColor; // map_Kd (options filename)
  MtlMap mapSpecularColor; // map_Ks (options filename)
  MtlMap mapSpecularExponent; // map_Ns (options filename)
  bool mapAntiAliasingTextures; // map_aat (on)
  MtlMap decal; // decal (options filename)
  MtlMap disposition; //disp (options filename)
  */
  // TODO: make sure we dont search beyond the first newmtl
  pos = data.find("Ka");
  if (pos != string::npos) {
    float ka[3];
    try {
      ka[0] = parseNextFloat(data, pos);
    } catch(MtlParseException& e) {
      cerr << "Failed parsing Ka";
      throw e;
    }
  }
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
  for (int i = 0; i < materials.size(); ++i) {
    if (materials[i]) {
      materials[i]->printProperties(" ", ((i + 1) == materials.size()));
    }
  }
}

