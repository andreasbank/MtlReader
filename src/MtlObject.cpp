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

void
MtlObject::skipToNextLine(const string& data, string::size_type& pos)
{
  string::size_type localPos = data.find_first_of("\n", pos);

  if (localPos != string::npos) {
    ++localPos;
    pos = localPos;
  }

  /* Skip whitespace */
  while ((data[localPos] == ' ') || (data[localPos] == '\t'))
    ++localPos;
}

void
MtlObject::parseParam3Floats(const string& data, string::size_type& pos,
    const string& param, string& opts, float value[])
{
  string::size_type localPos = pos;
  string::size_type lineEnd = data.find_first_of("\n", localPos);
  skipOptionalChars(data, localPos);
  if (((localPos = data.find(param, localPos)) != string::npos) &&
      (localPos < lineEnd)) {
    localPos += param.length();
    /* Extract first float */
    skipOptionalChars(data, localPos);
    string::size_type endPos = data.find(" ", localPos);
    string floatString = data.substr(localPos, endPos - localPos);
    value[0] = stof(floatString);
    localPos = endPos;
    /* Extract second float */
    skipOptionalChars(data, localPos);
    skipOptionalChars(data, endPos);
    endPos = data.find(" ", localPos);
    floatString = data.substr(localPos, endPos - localPos);
    value[1] = stof(floatString);
    localPos = endPos;
    /* Extract third float */
    skipOptionalChars(data, localPos);
    skipOptionalChars(data, endPos);
    endPos = data.find(" ", localPos);
    floatString = data.substr(localPos, endPos - localPos);
    value[2] = stof(floatString);
  } else {
    throw MtlParseException();
  }
}

void
MtlObject::parseParamFloat(const string& data, string::size_type& pos,
    const string& param, string& opts, float& value)
{
  string::size_type localPos = pos;
  string::size_type lineEnd = data.find_first_of("\n", localPos);
  skipOptionalChars(data, localPos);
  if (((localPos = data.find(param, localPos)) != string::npos) &&
      (localPos < lineEnd)) {
    localPos += param.length();
    skipOptionalChars(data, localPos);
    value = stof(data.substr(localPos, lineEnd - localPos));
  } else {
    throw MtlParseException();
  }
}
void
MtlObject::parseParamInt(const string& data, string::size_type& pos,
    const string& param, string& opts, int& value)
{
  string::size_type localPos = pos;
  string::size_type lineEnd = data.find_first_of("\n", localPos);
  skipOptionalChars(data, localPos);
  if (((localPos = data.find(param, localPos)) != string::npos) &&
      (localPos < lineEnd)) {
    localPos += param.length();
    skipOptionalChars(data, localPos);
    value = stoi(data.substr(localPos, lineEnd - localPos));
  } else {
    throw MtlParseException();
  }
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

  typedef enum mtlKeyType {
    KT_KA,
    KT_KD,
    KT_KS,
  } mtlKeyType;

  typedef enum mtlValType {
    VT_FLOAT,
    VT_2FLOATS,
    VT_3FLOATS,
    VT_INT,
    VT_2INTS,
    VT_3INTS
  } mtlValType;

  typedef struct mtlKey {
    const char* key;
    const mtlKeyType keyType;
    const mtlValType valType;
  } mtlKey;

  static const mtlKey keys[] = {
      { "Ka", KT_KA, VT_3FLOATS },
      { "Kd", KT_KD, VT_3FLOATS },
      { "Ks", KT_KS, VT_3FLOATS }
  };

  string opts;
  float fval[3];
  int ival[3];

  for (const auto& k : keys) {
    try {
      /* Parse by key type */
      switch (k.valType) {
      case VT_3FLOATS:
        cout << "Parsing " << k.key << endl;
        parseParam3Floats(data, pos, k.key, opts, fval);
        break;
      case VT_INT:
        cout << "Parsing " << k.key << endl;
        parseParamInt(data, pos, k.key, opts, ival[0]);
        break;
      default:
        cerr << "Fatal error, invalid value type" << endl;
      }

      /* Set to appropriate field in material object */
      switch (k.keyType) {
      case KT_KA:
        mat.ambientColor = { fval[0], fval[1], fval[2] };
        break;
      default:
        cerr << "Fatal error, invalid key type" << endl;
      }
    } catch(exception& e) {
      cerr << "Failed parsing '" << k.key << "' value(s) from material '" <<
          mat.name << "' (Object '" << mFileName  << "')" << endl;
      mat.ambientColor = { 0.5f, 0.5f, 0.5f };
    }
  }

#if 0
  try {
    /* Parse diffuse color */
    parseParam3Floats(data, pos, "Kd", opts, fval);
    mat.diffuseColor = { fval[0], fval[1], fval[2] };
    skipToNextLine(data, pos);
  } catch(exception& e) {
    cerr << "Failed parsing 'Kd' values from material '" <<
        mat.name << "' (Object '" << mFileName  << "')" << endl;
    mat.diffuseColor = { 0.5f, 0.5f, 0.5f };
  }

  try {
    /* Parse specular color */
    parseParam3Floats(data, pos, "Ks", opts, fval);
    mat.specularColor = { fval[0], fval[1], fval[2] };
    skipToNextLine(data, pos);
  } catch(exception& e) {
    cerr << "Failed parsing 'Ka', 'Kd' or 'Ks' values from material '" <<
        mat.name << "' (Object '" << mFileName  << "')" << endl;
    mat.specularColor = { 0.5f, 0.5f, 0.5f };
  }

  try {
    /* Parse illumination */
    parseParamInt(data, pos, "illum", opts, ival[0]);
    mat.illumination = ival[0];
    skipToNextLine(data, pos);
  } catch(exception& e) {
    cerr << "Failed parsing 'illum' value from material '" << mat.name <<
        "' (Object '" << mFileName  << "')" << endl;
    mat.illumination = 1;
  }

  try {
    /* Parse dissolve*/
    parseParamFloat(data, pos, "d", opts, fval[0]);
    mat.dissolve = fval[0];
    skipToNextLine(data, pos);
  } catch(exception& e) {
    cerr << "Failed parsing 'd' value from material '" << mat.name <<
        "' (Object '" << mFileName  << "')" << endl;
    mat.dissolve = 0;
  }

  try {
    /* Parse specular exponent */
    parseParamInt(data, pos, "Ns", opts, ival[0]);
    mat.specularExponent = ival[0];
    skipToNextLine(data, pos);
  } catch(exception& e) {
    cerr << "Failed parsing 'Ns' value from material '" << mat.name <<
        "' (Object '" << mFileName  << "')" << endl;
    mat.specularExponent = 5;
  }
#endif
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

  int i = 0;
  while (true && i++ < 10) {
    pos = data.find(MATERIAL_SENINTEL, pos);
    string::size_type mtlNameEnd = data.find("\n", pos);
    if ((pos == string::npos) || (mtlNameEnd == string::npos)) {
      break;
    }
    pos += MATERIAL_SENINTEL_LEN;

    MtlMaterial *mat = new MtlMaterial();
    skipOptionalChars(data, pos);
    mat->parent = this;
    mat->name = data.substr(pos, mtlNameEnd - pos);
    skipToNextLine(data, pos);
    parseMtlColorAndIllumination(data, pos, *mat);
    parseMtlTextureAndReflectionMaps(data, pos, *mat);
    materials.push_back(mat);
  }
}

void
MtlObject::printMaterials(void)
{
  cout << "Object '" << mFileName << "'" << endl;
  for (unsigned int i = 0; i < materials.size(); ++i) {
    if (materials[i]) {
      materials[i]->printProperties(" ", ((i + 1) == materials.size()));
    }
  }
}

