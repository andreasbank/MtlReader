/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#include <string>

#include "MtlMaterial.hpp"

using namespace std;

MtlMaterial::MtlMaterial(const std::string& matName) :
    name(matName), illumination(0), dissolve(0.0f),
    specularExponent(0), sharpness(0.0f), opticalDensity(0.0f),
    mapAmbientColor("ambient"), mapDiffuseColor("diffuse"),
    mapSpecularColor("specular color"),
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
  transformFilter.red = 0.0f;
  transformFilter.green = 0.0f;
  transformFilter.blue = 0.0f;
}

void
MtlMaterial::printProperties(const string& prefix, bool isLast)
{
  string localPrefix(string((isLast ? " " : "│")) + " ├─");

  cout << prefix << (isLast ? "└─" : "├─") << "Material name: " << name << endl;
  cout << prefix << localPrefix << "ambientColor (Ka): " << ambientColor.red << ", " <<
      ambientColor.green << ", " << ambientColor.blue << endl;
  cout << prefix << localPrefix << "diffuseColor (Kd): " << diffuseColor.red << ", " <<
      diffuseColor.green << ", " << diffuseColor.blue << endl;
  cout << prefix << localPrefix << "specularColor (Ks): " << specularColor.red << ", " <<
      specularColor.green << ", " << specularColor.blue << endl;
  cout << prefix << localPrefix << "Transform filter (Tf): " <<
      transformFilter.red << ", " << transformFilter.green << ", " <<
      transformFilter.blue << endl;
  cout << prefix << localPrefix << "illumination (illum): " << illumination << endl;
  cout << prefix << localPrefix << "dissolve (d): " << dissolve << endl;
  cout << prefix << localPrefix << "specularExponent (Ns): " << specularExponent << endl;
  cout << prefix << localPrefix << "sharpness (sharpness): " << sharpness << endl;
  cout << prefix << localPrefix << "opticalDensity (Ni): " << opticalDensity << endl;
  mapAmbientColor.printProperties(prefix + (isLast ? " " : "│"));
  mapDiffuseColor.printProperties(prefix + (isLast ? " " : "│"));
  mapSpecularColor.printProperties(prefix + (isLast ? " " : "│"));
  mapSpecularExponent.printProperties(prefix + (isLast ? " " : "│"));
  cout << prefix << localPrefix << "mapAntiAliasingTextures (map_aat): " <<
      mapAntiAliasingTextures << endl;
  decal.printProperties(prefix + (isLast ? " " : "│"));
  disposition.printProperties(prefix + (isLast ? " " : "│"), true);
}
