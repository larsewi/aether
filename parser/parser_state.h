#ifndef _AETHER_PARSER_SATE
#define _AETHER_PARSER_SATE

#include <stdint.h>
#include <stdlib.h>

/****************************************************************************/

typedef struct {
  int line;
  int col;
} ParserState;

extern ParserState PARSER_STATE;

/****************************************************************************/

typedef enum {
  ATOM_TYPE_FLOAT,
  ATOM_TYPE_STRING,
  ATOM_TYPE_INTEGER,
} AtomType;

typedef struct {
  int line;
  int col;
  AtomType type;
  union {
    uint64_t integer_literal;
    double float_literal;
    char *string_literal;
  } literal;
} Atom;

/****************************************************************************/

typedef enum {
  FACTOR_PREFIX_TYPE_PLUS,
  FACTOR_PREFIX_TYPE_MINUS,
} FactorPrefixType;

typedef struct {
  int line;
  int col;
  FactorPrefixType type;
} FactorPrefix;

/****************************************************************************/

#endif // _AETHER_PARSER_SATE
