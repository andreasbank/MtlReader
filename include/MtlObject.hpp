/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#ifndef MTLOBJECT_HPP
#define MTLOBJECT_HPP

#include <list>
#include <string>

#include "MtlMap.hpp"
#include "MtlMaterial.hpp"

class MtlObject {
public:
  MtlObject(const std::string& fileName);
  ~MtlObject(void);
  void skipOptionalChars(const std::string& data, std::string::size_type& pos);
  void parseMtlColorAndIllumination(const std::string& data,
      std::string::size_type& pos, MtlMaterial& mat);
  void parseMtlTextureAndReflectionMaps(const std::string& data,
      std::string::size_type& pos, MtlMaterial& mat);
  void parseMtlData(const std::string& data);
  void printMaterials(void);

  std::string mFileName;
  std::list<MtlMaterial *> materials;
};


#endif /* MTLOBJECT_HPP */