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
  union literal {
    uint64_t integer_literal;
    double float_literal;
    char *string_literal;
  } literal;
} Atom;

/****************************************************************************/

#endif // _AETHER_PARSER_SATE
