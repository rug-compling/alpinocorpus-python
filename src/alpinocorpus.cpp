#include <Python.h>

#include "CorpusReader.hh"
#include "Entry.hh"
#include "EntryIterator.hh"
#include "MarkerQuery.hh"

void raise_exception(char const *msg)
{
  PyObject *msgObj = Py_BuildValue("s", msg);
  PyErr_SetObject(PyExc_RuntimeError, msgObj);
}

static PyMethodDef AlpinoCorpusMethods[] = {
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initalpinocorpus(void)
{
  PyObject *m;

  if (PyType_Ready(&CorpusReaderType) < 0)
    return;
  if (PyType_Ready(&EntryIteratorType) < 0)
    return;
  if (PyType_Ready(&EntryType) < 0)
    return;
  if (PyType_Ready(&MarkerQueryType) < 0)
    return;

  m = Py_InitModule("alpinocorpus", AlpinoCorpusMethods);
  if (m == NULL)
    return;
  
  Py_INCREF(&CorpusReaderType);
  PyModule_AddObject(m, "CorpusReader", (PyObject *) &CorpusReaderType);
  PyModule_AddObject(m, "EntryIterator", (PyObject *) &EntryIteratorType);
  PyModule_AddObject(m, "Entry", (PyObject *) &EntryType);
  PyModule_AddObject(m, "MarkerQuery", (PyObject *) &MarkerQueryType);
}

