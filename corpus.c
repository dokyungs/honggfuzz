#include <stdio.h>
#include <string.h>

#include "libhfcommon/log.h"
#include "libhfcommon/files.h"
#include "corpus.h"

void corpus_writeEvolutionVector(const char *filepath, dynfile_t *new, dynfile_t *base, const char *mangleFuncStr) {
    FILE* fDot = fopen(filepath, "a+");
    if (fDot == NULL) {
      PLOG_W("Couldn't open '%s' - APPEND mode", filepath);
      return;
    }

    // node
    char newNodeName[PATH_MAX];
    memcpy(newNodeName, new->path, strlen(new->path));
    for (unsigned i=0; i<strlen(newNodeName); i++) {
      if (newNodeName[i] == '.') newNodeName[i] = '_';
      if (newNodeName[i] == '-') newNodeName[i] = '_';
    }
    char baseNodeName[PATH_MAX];
    memcpy(baseNodeName, base->path, strlen(base->path));
    for (unsigned i=0; i<strlen(baseNodeName); i++) {
      if (baseNodeName[i] == '.') baseNodeName[i] = '_';
      if (baseNodeName[i] == '-') baseNodeName[i] = '_';
    }

    char newNode[1 + PATH_MAX + 9 + PATH_MAX + 4 + 1];
    snprintf(newNode, sizeof(newNode), "n%s [label=\"%s\"];\n", newNodeName, new->path);
    fwrite(newNode, sizeof(char), strlen(newNode), fDot);

    // edge
    char newEdge[1 + PATH_MAX + 4 + 1 + PATH_MAX + 2 + 1];
    snprintf(newEdge, sizeof(newEdge), "n%s -> n%s [label=\"%s\"];\n", baseNodeName, newNodeName, mangleFuncStr);
    fwrite(newEdge, sizeof(char), strlen(newEdge), fDot);

#if 0 // append mode doesn't work
    fseek(fDot, 0L, SEEK_SET);
    fseek(fDot, 0L, SEEK_END);
    size_t sz = ftell(fDot);

    if (sz == 0) {
      const char header[] = "digraph D {\n";
      fwrite(header, sizeof(char), sizeof(header), fDot);
    }
    else {
      fseek(fDot, 0L, SEEK_SET);
      fseek(fDot, sz-3, SEEK_SET);
    }
    LOG_I("sz: %lu, ftell: %lu", sz, ftell(fDot));

    const char footer[] = "}\n";
    fwrite(footer, sizeof(char), sizeof(footer), fDot);
#endif

    fclose(fDot);

    LOG_I("%s -> %s", base->path, new->path);
}
