#include "sector.h"

typedef struct {
    PyObject_HEAD
    struct Sector *sector;
} Sector;
