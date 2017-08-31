/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#include <string>

#include "MtlMaterial.hpp"

using namespace std;

MtlMaterial::MtlMaterial(const std::string& matName) :
    name(matName),
    illumination(0), dissolve(0.0f), specularExponent(0), sharpness(0.0f),
    opticalDensity(0.0f), mapAmbientColor("ambient"),
    mapDiffuseColor("diffuse"), mapSpecularColor("specular color"),
    mapSpecularExponent("specular exponent"), mapAntiAliasingTextures(false),
    decal("decal"), disposition("disposition")
{
  ambientColor.red = 0.0f;
  ambientColor.green = 0.0f;
  ambientColor.blue = 0.0f;
  diffuseColor.red = 0.0f;
  diffuseColor.green = 0.0f;
  diffuseColor.blue = 0.0f;
  specularColor.red = 0.0f;
  specularColor.green = 0.0f;
  specularColor.blue = 0.0f;
}

void
MtlMaterial::printProperties(int tableLevel, bool isLast)
{
  string spaces;

  for (int i = 0; i < tableLevel; ++i) {
    spaces += " ";
  }

  cout << spaces << (isLast ? "└─" : "├─") << "Material name: " << name << endl;
  spaces += (isLast ? " " : "│") + string(" ├─");
  cout << spaces << "ambientColor (Ka): " << ambientColor.red << ", " <<
      ambientColor.green << ", " << ambientColor.blue << endl;
  cout << spaces << "diffuseColor (Kd): " << diffuseColor.red << ", " <<
      diffuseColor.green << ", " << diffuseColor.blue << endl;
  cout << spaces << "specularColor (Ks): " << specularColor.red << ", " <<
      specularColor.green << ", " << specularColor.blue << endl;
  cout << spaces << "illumination (illum): " << illumination << endl;
  cout << spaces << "dissolve (d): " << dissolve << endl;
  cout << spaces << "specularExponent (Ns): " << specularExponent << endl;
  cout << spaces << "sharpness (sharpness): " << sharpness << endl;
  cout << spaces << "opticalDensity (Ni): " << opticalDensity << endl;
  mapAmbientColor.printProperties(tableLevel + 2);
  mapDiffuseColor.printProperties(tableLevel + 2);
  mapSpecularColor.printProperties(tableLevel + 2);
  mapSpecularExponent.printProperties(tableLevel + 2);
  cout << spaces << "mapAntiAliasingTextures (map_aat): " <<
      mapAntiAliasingTextures << endl;
  decal.printProperties(tableLevel + 2);
  disposition.printProperties(tableLevel + 2, true);
}
