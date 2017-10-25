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
  void printMaterials(void);

  std::string mFileName;
  std::vector<MtlMaterial *> materials;

private:
  void skipOptionalChars(const std::string& data, std::string::size_type& pos);
  void skipToNextLine(const std::string& data, std::string::size_type& pos);
};


#endif /* MTLOBJECT_HPP */
