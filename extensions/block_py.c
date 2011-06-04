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
#include "structmember.h"

#include "blocktypes.h"

#include "block_py.h"


static PyObject *Block_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    return type->tp_alloc(type, 0);
}


static PyObject *Block_init(Block *self, PyObject *args, PyObject *kwds)
{
    unsigned short type, metadata, block_light, sun_light;
    if (!PyArg_ParseTuple(args, "hhhh", &type, &metadata, &block_light, &sun_light))
        return -1;

    self->blocktype = type;
    self->metadata = metadata;
    //TODO: lighting

    return 0;
}


static PyMethodDef Block_methods[] = {
    {NULL} // Sentinel
};


static PyObject *Block_get_block_light(Block *self, void *closure)
{
    return PyInt_FromLong(self->lighting >> 4); //TODO: check
}


static PyObject *Block_get_sun_light(Block *self, void *closure)
{
    return PyInt_FromLong(self->lighting & 0x0F); //TODO: check
}


static PyObject *Block_is_solid(Block *self, void *closure)
{
    return PyBool_FromLong(!(blocktypes[self->blocktype].flags & BLOCKTYPE_FLAG_NONSOLID));
}


static PyObject *Block_get_name(Block *self, void *closure)
{
    return PyUnicode_FromString(blocktypes[self->blocktype].name);
}


static PyGetSetDef Block_getseters[] = {
    {"block_light",
     (getter) Block_get_block_light, NULL,
     "TODO",
     NULL}, //TODO: setter
    {"sun_light",
     (getter) Block_get_sun_light, NULL,
     "TODO",
     NULL}, //TODO: setter
    {"solid",
     (getter) Block_is_solid, NULL,
     "TODO",
     NULL},
    {"name",
     (getter) Block_get_name, NULL,
     "TODO",
     NULL},
    {NULL}  /* Sentinel */
};


static PyMemberDef Block_members[] = {
    {"type", T_USHORT, offsetof(Block, blocktype), 0,
     "block type"},
    {"metadata", T_USHORT, offsetof(Block, metadata), 0,
     "block metadata"},
    {NULL}  /* Sentinel */
};


PyTypeObject BlockType = {
    PyObject_HEAD_INIT(NULL)
    .tp_name = "cmcclient.Block",
    .tp_basicsize = sizeof(Block),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "Block objects",
    .tp_init = (initproc) Block_init,
    .tp_new = Block_new,
    .tp_getset = Block_getseters,
    .tp_members = Block_members,
    .tp_methods = Block_methods,
};

