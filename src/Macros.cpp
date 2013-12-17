#include <Python.h>

#include <AlpinoCorpus/macros.hh>

#include "alpinocorpus.h"
#include "Macros.hh"

namespace ac = alpinocorpus;

static PyMethodDef Macros_methods[] = {
  {"expand", (PyCFunction) Macros_expand, METH_VARARGS, "Expand macros" },
  {NULL}
};

PyTypeObject MacrosType = {
            PyVarObject_HEAD_INIT(NULL, 0)
            "alpinocorpus.Macros",                    /* tp_name */
            sizeof(Macros),                           /* tp_basicsize */
            0,                                        /* tp_itemsize */
            (destructor)Macros_dealloc,               /* tp_dealloc */
            0,                                        /* tp_print */
            0,                                        /* tp_getattr */
            0,                                        /* tp_setattr */
            0,                                        /* tp_compare */
            0,                                        /* tp_repr */
            0,                                        /* tp_as_number */
            0,                                        /* tp_as_sequence */
            0,                                        /* tp_as_mapping */
            0,                                        /* tp_hash */
            0,                                        /* tp_call */
            0,                                        /* tp_str */
            0,                                        /* tp_getattro */
            0,                                        /* tp_setattro */
            0,                                        /* tp_as_buffer */
            Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
            "Macros(path)\n\n"
            "... where path refers to a macro file.", /* tp_doc */
            0,                                        /* tp_traverse */
            0,                                        /* tp_clear */
            0,                                        /* tp_richcompare */
            0,                                        /* tp_weaklistoffset */
            0,                                        /* tp_iter */
            0,                                        /* tp_iternext */
            Macros_methods,                           /* tp_methods */
            0,                                        /* tp_members */
            0,                                        /* tp_getset */
            0,                                        /* tp_base */
            0,                                        /* tp_dict */
            0,                                        /* tp_descr_get */
            0,                                        /* tp_descr_set */
            0,                                        /* tp_dictoffset */
            0,                                        /* tp_init */
            0,                                        /* tp_alloc */
            Macros_new,                               /* tp_new */
        };


PyObject *Macros_new(PyTypeObject *type, PyObject *args,
  PyObject *kwds)
{
  static char const *kwlist[] = {"macros", NULL};
  char *path = NULL;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", (char **) kwlist, &path))
    return NULL;

  Macros *self;
  self = (Macros *) type->tp_alloc(type, 0);
  try {
    self->macros = new alpinocorpus::Macros(
      alpinocorpus::loadMacros(path));
  } catch (std::runtime_error &e) {
    raise_exception(e.what());
    return NULL;
  }

  return (PyObject *) self;
}


void Macros_dealloc(Macros *self)
{
  delete self->macros;
  OB_TYPE(self)->tp_free(self);
}

PyObject *Macros_expand(Macros *self, PyObject *args) {
  char *query;
  if (!PyArg_ParseTuple(args, "s", &query))
    return NULL;

  std::string expanded;
  Py_BEGIN_ALLOW_THREADS
  try {
    expanded = ac::expandMacros(*self->macros, query);
  } catch (std::runtime_error &e) {
    Py_BLOCK_THREADS
    raise_exception(e.what());
    return NULL;
  }
  Py_END_ALLOW_THREADS

  return Py_BuildValue("s#", expanded.c_str(), expanded.size());
}
