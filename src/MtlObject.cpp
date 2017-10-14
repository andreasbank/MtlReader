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

typedef enum mtlKeyType {
  KT_KA,
  KT_KD,
  KT_KS,
  KT_ILLUM,
  KT_D,
  KT_NS,
  KT_SHARPNESS,
  KT_NI,
  KT_MAPKA,
  KT_MAPKD,
  KT_MAPKS,
  KT_MAPNS,
  KT_MAPD,
  KT_DISP,
  KT_DECAL,
  KT_BUMP,
  KT_MAPBUMP,
  KT_REFL,
} mtlKeyType;

typedef enum mtlValType {
  VT_FLOAT,
  VT_2FLOATS,
  VT_3FLOATS,
  VT_INT,
  VT_2INTS,
  VT_3INTS,
  VT_STRING
} mtlValType;

typedef struct mtlKey {
  const char* key;
  const mtlKeyType keyType;
  const mtlValType valType;
} mtlKey;

static const mtlKey keys[] = {
    { "Ka", KT_KA, VT_3FLOATS },
    { "Kd", KT_KD, VT_3FLOATS },
    { "Ks", KT_KS, VT_3FLOATS },
    { "illum", KT_ILLUM, VT_INT },
    { "d", KT_D, VT_FLOAT },
    { "Ns", KT_NS, VT_INT }
};

class MtlParseException : public exception {
   const char * what () const throw () {
      return "Failed parsing the file";
   }
};

MtlObject::MtlObject(const string& fileName) : mFileName(fileName)
{
  ifstream dataFile(fileName);
  string line;
  if (!dataFile.is_open()) {
    cerr << "Failed to open file '" << fileName << "'" << endl;
  }
  while (getline(dataFile, line))
    parseLine(line);
  dataFile.close();
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
MtlObject::parseLine(const string& data)
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
  string::size_type pos = 0;

  /* Always skip spaces */
  skipOptionalChars(data, pos);

  /* If we encounter a newmtl, create a new material object */
  if (pos == data.find(MATERIAL_SENINTEL, pos)) {
    pos += MATERIAL_SENINTEL_LEN;
    MtlMaterial *mat = new MtlMaterial();
    skipOptionalChars(data, pos);
    mat->parent = this;
    mat->name = data.substr(pos);
    materials.push_back(mat);
    cout << "Created material '" << mat->name << "'" << endl;
    return;
  }

  /* If no newmtl have been found previously then the file is erroneous,
     since we have nothing to add found properties to */
  if (!materials.size()) {
    cerr << "No material in Mtl file" << endl;
    return;
  }

  /* Use last 'newmtl' for all properties we parse */
  MtlMaterial& mat = *materials.back();

  string opts;
  float fval[3];
  int ival[3];

  /* Go through the list of valid keys */
  for (const auto& k : keys) {
    string::size_type localPos = 0;

    /* Try to match the parameter name with a valid key */
    if (((localPos = data.find(k.key, pos)) == string::npos) ||
        pos != localPos)
      continue;

    localPos += strlen(k.key);
    skipOptionalChars(data, localPos);

    try {
      /* Parse by key type */
      switch (k.valType) {
      case VT_FLOAT:
        cout << "Parsing " << k.key << endl;
        parseParamFloat(data, pos, k.key, opts, fval[0]);
        break;
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
      case KT_KD:
        mat.diffuseColor = { fval[0], fval[1], fval[2] };
        break;
      case KT_KS:
        mat.specularColor = { fval[0], fval[1], fval[2] };
        break;
      case KT_ILLUM:
        mat.illumination = ival[0];
        break;
      case KT_D:
        mat.dissolve = ival[0];
        break;
      case KT_NS:
        mat.specularExponent = ival[0];
        break;
      default:
        cerr << "Fatal error, invalid key type" << endl;
      }
    } catch(exception& e) {
      cerr << "Failed parsing '" << k.key << "' value(s) from material '" <<
          mat.name << "' (Object '" << mFileName  << "')" << endl;
    }
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

