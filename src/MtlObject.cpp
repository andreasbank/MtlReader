/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

#include "MtlObject.hpp"
#include "MtlObject_int.hpp"
#include "MtlObjectExceptions.hpp"

#define MATERIAL_SENINTEL "newmtl"
#define MATERIAL_SENINTEL_LEN 7

using namespace std;

MtlObject::MtlObject(const string& fileName) : mFileName(fileName)
{
  ifstream dataFile(fileName);
  string line;
  if (!dataFile.is_open()) {
    cerr << "Failed to open file '" << fileName << "'" << endl;
  }

  while (getline(dataFile, line))
  {
    cout << "^^^^ PARSING LINE" << endl;
    parseLine(line);
  }

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
    /* Skip whitespace */
    while ((data[localPos] == ' ') || (data[localPos] == '\t'))
      ++localPos;
    pos = localPos;
  }

}

void
MtlObject::parseParam3Floats(const string& data, string::size_type& pos,
    float value[])
{
  /* Extract first float */
  skipOptionalChars(data, pos);
  string::size_type endPos = data.find(" ", pos);
  value[0] = stof(data.substr(pos, endPos - pos));
  pos = endPos;
  /* Extract second float */
  skipOptionalChars(data, pos);
  endPos = data.find(" ", pos);
  value[1] = stof(data.substr(pos, endPos - pos));
  pos = endPos;
  /* Extract third float */
  skipOptionalChars(data, pos);
  endPos = data.find(" ", pos);
  value[2] = stof(data.substr(pos, endPos - pos));
  pos = endPos;
}

void
MtlObject::parseParamFloat(const string& data, string::size_type& pos,
    float& value)
{
  skipOptionalChars(data, pos);
  string::size_type left = 0;
  value = stof(data.substr(pos), &left);
  pos += left;
}

void
MtlObject::parseParamInt(const string& data, string::size_type& pos,
    int& value)
{
  skipOptionalChars(data, pos);
  string::size_type left = 0;
  value = stoi(data.substr(pos), &left);
  pos += left;
}

void
MtlObject::parseParamString(const string& data, string::size_type& pos,
    string& value)
{
  skipOptionalChars(data, pos);
  string::size_type left = 0;
  value = stoi(data.substr(pos), &left);
  pos += left;
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
    cerr << "No material yet in Mtl file, skipping line" << endl;
    return;
  }

  /* Use last 'newmtl' for all properties we parse */
  MtlMaterial& mat = *materials.back();

  string opts;

  /* Go through the list of valid keys */
  bool keyMatched = false;
  for (const auto& k : keys) {
    string::size_type keyNameSize = string(k.keyName).length();

    /* Try to match the parameter name with a valid key */
    if (((data.compare(pos, keyNameSize, k.keyName)) != 0) ||
        (data[keyNameSize] != ' '))
      continue;

    keyMatched = true;

    cout << "Matched key: " << k.keyName << endl;
    pos += keyNameSize;
    skipOptionalChars(data, pos);

    /* Go through all the possible values for this key */
    bool valueMatched = false;
    for (int i = 0; i < k.nrValues; ++i) {
      const mtlVal& v = k.values[i];

      /* Check if the value type matches */
      if (v.valName && data.compare(pos, string(
          const_cast<const char *>(v.valName)).size(), v.valName))
        continue;

      valueMatched = true;

      cout << "Matched value: " << (v.valName ? v.valName : "unnamed") << endl;
      try {
        float floatData[3];
        int intData[3];
        string stringData;

        /* Parse by value type */
        switch (v.valType) {
        case VT_FLOAT:
          cout << "Parsing float for " << k.keyName << endl;
          //parseOptions(data, pos, k.nrOptions, k.options, string
          parseParamFloat(data, pos, floatData[0]);
          break;
        case VT_3FLOATS:
          cout << "Parsing float[3] for " << k.keyName << endl;
          //parseOptions(data, pos, k.nrOptions, k.options, string
          parseParam3Floats(data, pos, floatData);
          break;
        case VT_INT:
          cout << "Parsing int for " << k.keyName << endl;
          //parseOptions(data, pos, k.nrOptions, k.options, string
          parseParamInt(data, pos, intData[0]);
          break;
        case VT_STRING_AND_FLOAT:
          cout << "Parsing int for " << k.keyName << endl;
          //parseOptions(data, pos, k.nrOptions, k.options, string
          parseParamString(data, pos, stringData);
          break;
        default:
          cerr << "Fatal error, invalid value type (\"" <<
              (v.valName ? v.valName : "unnamed") << "\", " << v.valType
              << ")" << endl;
          throw MtlParseException();
        }

        /* Set to appropriate field in material object */
        switch (k.keyType) {
        case KT_KA:
          mat.ambientColor = { floatData[0], floatData[1], floatData[2] };
          break;
        case KT_KD:
          mat.diffuseColor = { floatData[0], floatData[1], floatData[2] };
          break;
        case KT_KS:
          mat.specularColor = { floatData[0], floatData[1], floatData[2] };
          break;
        case KT_TF:
          mat.transformFilter = { floatData[0], floatData[1], floatData[2] };
          break;
        case KT_ILLUM:
          mat.illumination = intData[0];
          break;
        case KT_D:
          mat.dissolve = floatData[0];
          break;
        case KT_NS:
          mat.specularExponent = intData[0];
          break;
        default:
          cerr << "Fatal error, invalid key type (" << k.keyType << ")" <<
              endl;
        }
      } catch(exception& e) {
        cerr << "Failed parsing '" << k.keyName << "' value(s) from material '"
            << mat.name << "' (Object '" << mFileName  << "')" << endl;
      }

      /* If we have matched the parameters value with a valid key value then do
         not continue searching for a matching key value */
      if (valueMatched)
      {
        cout << "--- DONE VAL" << endl;
        break;
      }

    } /* for key values */

    /* If we have matched the parameter with a valid key then do not continue
       searching for a matching key */
    if (keyMatched)
    {
      cout << "--- DONE KEY" << endl;
      break;
    }

  } /* for keys */
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

