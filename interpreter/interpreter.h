#ifndef _AETHER_INTERPRETER_H
#define _AETHER_INTERPRETER_H

#include <stdbool.h>

#include "../parser/syntax.h"

void WalkSyntaxTree(SymbolStatement *statement, bool print_tree);

#endif // _AETHER_INTERPRETER_H
