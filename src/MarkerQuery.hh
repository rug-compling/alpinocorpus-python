#ifndef PYTHON_MARKERQUERY_HH
#define PYTHON_MARKERQUERY_HH

#include <string>

#include <Python.h>

typedef struct {
  PyObject_HEAD
  std::string *query;
  std::string *attr;
  std::string *value;
} MarkerQuery;

extern PyTypeObject MarkerQueryType;

PyObject *MarkerQuery_new(PyTypeObject *type, PyObject *args,
  PyObject *kwds);
void MarkerQuery_dealloc(MarkerQuery *self);


#endif // PYTHON_MARKERQUERY_HH
