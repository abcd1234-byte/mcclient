/*
 * Copyright (C) 2011 Thibaut GIRKA <thib@sitedethib.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 3 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <Python.h>

#include "block_py.h"
#include "sector_py.h"
#include "world_renderer_py.h"


static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC initcmcclient(void)
{
    PyObject* m;

    if (PyType_Ready(&WorldRendererType) < 0)
        return;
    if (PyType_Ready(&SectorType) < 0)
        return;
    if (PyType_Ready(&BlockType) < 0)
        return;

    m = Py_InitModule3("cmcclient", module_methods,
                       "TODO");

    if (m == NULL)
      return;

    Py_INCREF(&WorldRendererType);
    PyModule_AddObject(m, "WorldRenderer", (PyObject *) &WorldRendererType);

    Py_INCREF(&SectorType);
    PyModule_AddObject(m, "Sector", (PyObject *) &SectorType);

    Py_INCREF(&BlockType);
    PyModule_AddObject(m, "Block", (PyObject *) &BlockType);
}
