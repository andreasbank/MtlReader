/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#ifndef MTLMATERIAL_HPP
#define MTLMATERIAL_HPP

#include <string>

#include "MtlMap.hpp"

class MtlObject;

struct MtlColor {
  float red, green, blue;
};

class MtlMaterial {
public:
  MtlMaterial(const std::string& matName = std::string());
  void printProperties(const std::string& prefix = std::string(),
      bool isLast = false);

  MtlObject* parent;
  std::string name; // newmtl (string)
  MtlColor ambientColor, diffuseColor, specularColor; // Ka, Kd, Ks (3 * [0 - 1])
  MtlColor transformFilter; // Tf (3 * [0 - 1])
  int illumination; // illum (0 - 10) (predefined meaning, enum)
  float dissolve; // d (0.0 - 1.0)
  bool dissolveHalo; // -halo (true if 'd' has '-halo' option, else false)
  int specularExponent; // Ns (0 - 1000)
  int sharpness; // sharpness (0 - 1000)
  float opticalDensity; // Ni (0.001 - 10.0)
  MtlMap mapAmbientColor; // map_Ka (options filename)
  MtlMap mapDiffuseColor; // map_Kd (options filename)
  MtlMap mapSpecularColor; // map_Ks (options filename)
  MtlMap mapSpecularExponent; // map_Ns (options filename)
  bool mapAntiAliasingTextures; // map_aat (on)
  MtlMap decal; // decal (options filename)
  MtlMap disposition; //disp (options filename)
};

#endif /* MTLMATERIAL_HPP */
