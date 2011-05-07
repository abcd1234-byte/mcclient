#include <Python.h>

#include "world_renderer.h"
#include "sector_py.h"

#include "frustum.h"

typedef struct {
    PyObject_HEAD
    struct WorldRenderer *world_renderer;
} WorldRenderer;


static void WorldRenderer_dealloc(WorldRenderer *self)
{
    Py_DECREF(self->world_renderer->sectors_dict);
    free(self->world_renderer);
    self->ob_type->tp_free((PyObject *) self);
}


static PyObject *WorldRenderer_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    WorldRenderer *self;

    self = (WorldRenderer *) type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->world_renderer = world_renderer_new();
        Py_INCREF(Py_None);
        self->world_renderer->sectors_dict = Py_None;
        Py_INCREF(Py_None);
        self->world_renderer->get_block_texture = Py_None;
    }

    return (PyObject *) self;
}


static int WorldRenderer_init(WorldRenderer *self, PyObject *args, PyObject *kwds)
{
    PyObject *sectors_dict = NULL, *get_block_texture, *tmp = NULL;

    if (!PyArg_ParseTuple(args, "OO", &sectors_dict, &get_block_texture))
        return -1;

    tmp = self->world_renderer->sectors_dict;
    Py_INCREF(sectors_dict);
    self->world_renderer->sectors_dict = sectors_dict;
    Py_XDECREF(tmp);

    tmp = self->world_renderer->get_block_texture;
    Py_INCREF(get_block_texture);
    self->world_renderer->get_block_texture = get_block_texture;
    Py_XDECREF(tmp);

    return 0;
}


static PyObject *WorldRenderer_render(WorldRenderer *self, PyObject *args)
{
    struct ViewContext view_context;
    struct Vec3D pos;
    double ratio, znear, zfar, yaw, pitch, fov;
    PyObject *iterable = NULL;
    PyObject *iterator = NULL;
    PyObject *item = NULL;

    if (!PyArg_ParseTuple(args, "(ddd)dddddd",
                          &view_context.x, &view_context.y, &view_context.z,
                          &fov, &ratio, &znear, &zfar, &yaw, &pitch))
        return -1;

    world_renderer_reset_rendering(self->world_renderer);

    pos.x = view_context.x; pos.y = view_context.y; pos.z = view_context.z;
    make_frustum(fov * M_PI / 180, ratio, znear, zfar, pos, yaw * M_PI / 180, pitch * M_PI / 180, view_context.frustum);

    iterable = PyDict_Values(self->world_renderer->sectors_dict);
    iterator = PyObject_GetIter(iterable);
    if (iterator == NULL)
        return NULL;

    while ((item = PyIter_Next(iterator)) != NULL)
    {
        //TODO: check object type
/*        if (!PyObject_TypeCheck(item, SectorType))
        {
            Py_DECREF(item);
            PyErr_SetString(PyExc_TypeError, "Give me a sector!");
            break;
        }*/
        world_renderer_render_sector(self->world_renderer, ((Sector*) item)->sector, &view_context);
        Py_DECREF(item);
    }

    Py_DECREF(iterator);
    Py_DECREF(iterable);

    if (PyErr_Occurred())
        return NULL;

    Py_INCREF(Py_None);
    return Py_None;
}


static PyMethodDef WorldRenderer_methods[] = {
    {"render", (PyCFunction) WorldRenderer_render, METH_VARARGS,
     "Render given sectors."},
    {NULL} // Sentinel
};


static PyObject *WorldRenderer_get_vertices(WorldRenderer *self, void *closure)
{
    return PyBytes_FromStringAndSize((const char *) self->world_renderer->vertices,
                                      sizeof(struct vertex) * self->world_renderer->nb_vertices);
}


static PyObject *WorldRenderer_get_colors(WorldRenderer *self, void *closure)
{
    return PyBytes_FromStringAndSize((const char *) self->world_renderer->colors,
                                      sizeof(struct color) * self->world_renderer->nb_vertices);
}


static PyObject *WorldRenderer_get_texcoords(WorldRenderer *self, void *closure)
{
    return PyBytes_FromStringAndSize((const char *) self->world_renderer->texcoords,
                                      sizeof(struct uv) * self->world_renderer->nb_vertices);
}


static PyObject *WorldRenderer_get_nb_vertices(WorldRenderer *self, void *closure)
{
    return PyInt_FromLong(self->world_renderer->nb_vertices);
}


static PyGetSetDef WorldRenderer_getseters[] = {
    {"nb_vertices", 
     (getter)WorldRenderer_get_nb_vertices, NULL,
     "Number of vertices to dispaly",
     NULL},
    {"vertices", 
     (getter)WorldRenderer_get_vertices, NULL,
     "Number of vertices to dispaly",
     NULL},
    {"colors", 
     (getter)WorldRenderer_get_colors, NULL,
     "Number of vertices to dispaly",
     NULL},
    {"texcoords", 
     (getter)WorldRenderer_get_texcoords, NULL,
     "Number of vertices to dispaly",
     NULL},
    {NULL}  /* Sentinel */
};


static PyTypeObject WorldRendererType = {
    PyObject_HEAD_INIT(NULL)
    .tp_name = "worldrenderer.WorldRenderer",
    .tp_basicsize = sizeof(WorldRenderer),
    .tp_dealloc = (destructor) WorldRenderer_dealloc,
    .tp_methods = WorldRenderer_methods,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "World Renderers",
    .tp_new = WorldRenderer_new,
    .tp_init = (initproc) WorldRenderer_init,
    .tp_getset = WorldRenderer_getseters,
};



static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initworldrenderer(void) 
{
    PyObject* m;

    if (PyType_Ready(&WorldRendererType) < 0)
        return;

    m = Py_InitModule3("worldrenderer", module_methods,
                       "TODO");

    if (m == NULL)
      return;

    Py_INCREF(&WorldRendererType);
    PyModule_AddObject(m, "WorldRenderer", (PyObject *)&WorldRendererType);
}
