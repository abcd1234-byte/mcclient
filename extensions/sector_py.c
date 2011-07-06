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

#include "sector.h"

#include "block_py.h"
#include "sector_py.h"


static void Sector_dealloc(Sector *self)
{
    if (self->sector->east != NULL && self->sector->east->west == self->sector)
        self->sector->east->west = NULL;
    if (self->sector->west != NULL && self->sector->west->east == self->sector)
        self->sector->west->east = NULL;
    if (self->sector->south != NULL && self->sector->south->north == self->sector)
        self->sector->south->north = NULL;
    if (self->sector->north != NULL && self->sector->north->south == self->sector)
        self->sector->north->south = NULL;

    free(self->sector);
    self->ob_type->tp_free((PyObject *) self);
}


static PyObject *Sector_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Sector *self;

    self = (Sector *) type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->sector = NULL;
    }

    return (PyObject *) self;
}


static PyObject *Sector_init(Sector *self, PyObject *args, PyObject *kwds)
{
    int cx, cz;
    if (!PyArg_ParseTuple(args, "ii", &cx, &cz))
        return -1;

    self->sector = sector_new(cx, cz);

    return 0;
}


static PyObject *Sector_add_neighbour(Sector *self, PyObject *args)
{
    int dx = 0, dz = 0;
    PyObject *neighbour_pointer = NULL;
    Sector *neighbour = NULL;

    if (!PyArg_ParseTuple(args, "O(ii)", &neighbour_pointer, &dx, &dz))
        return NULL;

    if (!PyObject_TypeCheck(neighbour_pointer, &SectorType))
    {
        PyErr_SetString(PyExc_TypeError, "neighbour must be a Sector");
        return NULL;
    }

    neighbour = (Sector *) neighbour_pointer;

    if (dx == -1)
    {
        self->sector->north = neighbour->sector;
        self->sector->north->south = self->sector;
    }
    if (dx == 1)
    {
        self->sector->south = neighbour->sector;
        self->sector->south->north = self->sector;
    }
    if (dz == -1)
    {
        self->sector->east = neighbour->sector;
        self->sector->east->west = self->sector;
    }
    if (dz == 1)
    {
        self->sector->west = neighbour->sector;
        self->sector->west->east = self->sector;
    }

    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *Sector_set_chunk(Sector *self, PyObject *args)
{
    int start_x = 0, start_y = 0, start_z = 0, size_x = 0, size_y = 0, size_z = 0;
    int data_size = 0;
    const unsigned char *data = NULL;

    if (!PyArg_ParseTuple(args, "iiiiiis#", &start_x, &start_y, &start_z,
                                            &size_x, &size_y, &size_z,
                                            &data, &data_size))
        return NULL;

    sector_set_chunk(self->sector, start_x, start_y, start_z,
                                   size_x, size_y, size_z,
                                   data);

    sector_gen_faces(self->sector);
    sector_update_boundaries(self->sector);

    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *Sector_set_block(Sector *self, PyObject *args)
{
    unsigned short x = 0, y = 0, z = 0;
    unsigned char blocktype, blockdata;

    if (!PyArg_ParseTuple(args, "hhhbb", &x, &y, &z, &blocktype, &blockdata))
        return NULL;

    sector_set_block(self->sector, x, y, z, blocktype, blockdata);

    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *Sector_get_block(Sector *self, PyObject *args)
{
    unsigned short x, y, z;
    PyObject *arglist = NULL;
    PyObject *result = NULL;

    if (!PyArg_ParseTuple(args, "hhh", &x, &y, &z))
        return NULL;

    if (x >= 0 && x < 16 && y >= 0 && y < 128 && z >= 0 && z < 16)
    {
        arglist = Py_BuildValue("hhhh", self->sector->blocktypes[x][z][y],
                                        self->sector->blockdata[x][z][y],
                                        self->sector->lighting[x][z][y] >> 4,
                                        self->sector->lighting[x][z][y] & 0x0F);
        result = PyObject_CallObject((PyObject *) &BlockType, arglist);
        Py_DECREF(arglist);

        return result;
    }
    else
    {
        PyErr_SetString(PyExc_IndexError, "block index out of range");
        return NULL;
    }
}


static PyMethodDef Sector_methods[] = {
    {"set_chunk", (PyCFunction) Sector_set_chunk, METH_VARARGS,
     "Set data chunk (TODO)."},
    {"set_block", (PyCFunction) Sector_set_block, METH_VARARGS,
     "Set block type and metadata (TODO)."},
    {"add_neighbour", (PyCFunction) Sector_add_neighbour, METH_VARARGS,
     "Add a neighbour to the sector."},
    {"get_block", (PyCFunction) Sector_get_block, METH_VARARGS,
     "Get a given block (in sector coordinates) from the sector."},
    {NULL} // Sentinel
};



PyTypeObject SectorType = {
    PyObject_HEAD_INIT(NULL)
    .tp_name = "cmcclient.Sector",
    .tp_basicsize = sizeof(Sector),
    .tp_dealloc = (destructor) Sector_dealloc,
    .tp_methods = Sector_methods,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "Sector objects",
    .tp_init = (initproc)Sector_init,
    .tp_new = Sector_new,
};

