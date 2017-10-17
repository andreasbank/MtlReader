/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#ifndef _MTLOBJECTEXCEPTIONS_HPP_
#define _MTLOBJECTEXCEPTIONS_HPP_

#include <exception>

using namespace std;

class MtlParseException : public exception {
   const char * what () const throw () {
      return "Failed parsing the file";
   }
};

#endif /* _MTLOBJECTEXCEPTIONS_HPP_ */
