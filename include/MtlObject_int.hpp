/**
 * @copyright 2017 Andreas Bank, andreas.mikael.bank@gmail.com
 */

#ifndef _MTLOBJECT_INT_HPP_
#define _MTLOBJECT_INT_HPP_

typedef enum mtlKeyType {
  KT_KA,
  KT_KD,
  KT_KS,
  KT_TF,
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
  VT_STRING,
  VT_STRING_AND_FLOAT,
} mtlValType;

typedef mtlValType mtlOptType;

typedef struct mtlOpt {
    const char* optName;
    const mtlOptType optType;
} mtlOpt;

typedef struct mtlVal {
  const char* valName;
  const mtlValType valType;
} mtlVal;

typedef struct mtlKey {
  const char* keyName;
  const mtlKeyType keyType;
  const int nrOptions;
  const mtlOpt* options;
  const int nrValues;
  const mtlVal* values;
} mtlKey;

/** Ka, Kd, Ks and Tf 'Possible values' struct-array */
static const mtlVal kVals[] = {
    { "xyz", VT_3FLOATS },
    { "spectral", VT_STRING_AND_FLOAT },
    { nullptr, VT_3FLOATS }
};

/** 'd' options (-halo) */
static const mtlOpt dOpts = { "-halo", VT_FLOAT };

/** Possible values - single int */
static const mtlVal intVal = { nullptr, VT_INT };

/** Possible values - single float */
static const mtlVal floatVal = { nullptr, VT_FLOAT };

/** A structure describing the possible MTL file material
    parameters and its options */
static const mtlKey keys[] = {

    { "Ka", KT_KA,
      0, /* Number of options */
      nullptr, /* Options (new mtlOpt{ { "-o", VT_3FLOATS }, ... }) */
      3, /* Number of possible value-types */
      kVals /* Possible values */
    },

    { "Kd", KT_KD, 0, nullptr, 3, kVals },

    { "Ks", KT_KS, 0, nullptr, 3, kVals },

    { "Tf", KT_TF, 0, nullptr, 3, kVals },

    { "illum", KT_ILLUM, 0, nullptr, 1, &intVal },

    { "d", KT_D, 1, &dOpts, 1, &floatVal },

    { "Ns", KT_NS, 0, nullptr, 1, &intVal },
};

#endif /* _MTLOBJECT_INT_HPP_ */
