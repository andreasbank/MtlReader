/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#ifndef MTLOBJECT_HPP
#define MTLOBJECT_HPP

#include <string>
#include <vector>

#include "MtlMap.hpp"
#include "MtlMaterial.hpp"

class MtlObject {
public:
  MtlObject(const std::string& fileName);
  ~MtlObject(void);
  void skipOptionalChars(const std::string& data, std::string::size_type& pos);
  void skipToNextLine(const std::string& data, std::string::size_type& pos);
  void parseOptions(const std::string& data, std::string::size_type& pos,
      void *pret);
  void parseParam3Floats(const std::string& data, std::string::size_type& pos,
      float value[]);
  void parseParamFloat(const std::string& data, std::string::size_type& pos,
      float& value);
  void parseParamInt(const std::string& data, std::string::size_type& pos,
      int& value);
  void parseLine(const std::string& data);
  void printMaterials(void);

  std::string mFileName;
  std::vector<MtlMaterial *> materials;
};


#endif /* MTLOBJECT_HPP */
