#ifndef HY_QUERY_INTERNAL_H
#define HY_QUERY_INTERNAL_H

// libsolv
#include <solv/bitmap.h>

// hawkey
#include "query.h"

union _Match {
    int num;
    HyPackageSet pset;
    HyReldep reldep;
    char *str;
};

enum _match_type {
    _HY_VOID,
    _HY_NUM,
    _HY_PKG,
    _HY_RELDEP,
    _HY_STR,
};

struct _Filter {
    int cmp_type;
    int keyname;
    int match_type;
    union _Match *matches;
    int nmatches;
    char *evr;
};

struct _HyQuery {
    HySack sack;
    Map *result;
    struct _Filter *filters;
    int nfilters;
    int downgrades; /* 1 for "only downgrades for installed packages" */
    int updates; /* 1 for "only updates for installed packages" */
    int latest; /* 1 for "only the latest version per arch" */
};

struct _Filter *filter_create(int nmatches);
void filter_reinit(struct _Filter *f, int nmatches);
void filter_free(struct _Filter *f);

#endif // HY_QUERY_INTERNAL_H
