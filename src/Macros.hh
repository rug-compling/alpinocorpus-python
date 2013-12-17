#ifndef PYTHON_MACROS_HH
#define PYTHON_MACROS_HH

#include <Python.h>

#include <AlpinoCorpus/macros.hh>


struct Macros {
  PyObject_HEAD
  alpinocorpus::Macros *macros;
};

extern PyTypeObject MacrosType;

PyObject *Macros_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
void Macros_dealloc(Macros *self);
PyObject *Macros_expand(Macros *self, PyObject *args);

#endif // PYTHON_MACROS_HH
