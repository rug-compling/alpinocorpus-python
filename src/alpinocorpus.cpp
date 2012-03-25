#include <Python.h>

#include "alpinocorpus.h"
#include "CorpusReader.hh"
#include "Entry.hh"
#include "EntryIterator.hh"
#include "MarkerQuery.hh"

extern "C" {
#include <libxslt/xslt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libexslt/exslt.h>
}

void raise_exception(char const *msg)
{
  PyObject *msgObj = Py_BuildValue("s", msg);
  PyErr_SetObject(PyExc_RuntimeError, msgObj);
}

static PyMethodDef AlpinoCorpusMethods[] = {
  {NULL, NULL, 0, NULL}
};

// XXX - Does this conflict in any way with libxml2/lbxslt module
//       cleanups?
void cleanup()
{
  xsltCleanupGlobals();
  xmlCleanupParser();
}

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef moduledef = {
  PyModuleDef_HEAD_INIT,
  "alpinocorpus",
  NULL,
  0,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};
#endif

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_alpinocorpus(void)
#else
PyMODINIT_FUNC initalpinocorpus(void)
#endif
{
  static int initialized = 0;

  PyObject *m;

  if (PyType_Ready(&CorpusReaderType) < 0)
    MODULE_RETURN(Py_None)
  if (PyType_Ready(&EntryIteratorType) < 0)
    MODULE_RETURN(Py_None)
  if (PyType_Ready(&EntryType) < 0)
    MODULE_RETURN(Py_None)
  if (PyType_Ready(&MarkerQueryType) < 0)
    MODULE_RETURN(Py_None)
  if (initialized != 0)
    MODULE_RETURN(Py_None)

#if PY_MAJOR_VERSION >= 3
  m = PyModule_Create(&moduledef);
  if (m == NULL)
    return Py_None;
#else
  m = Py_InitModule("alpinocorpus", AlpinoCorpusMethods);
  if (m == NULL)
    return;
#endif
  
  initialized = 1;

  // libxml2/libxslt initialization
  xmlInitMemory();
  xmlInitParser();
  xmlXPathInit();
  exsltRegisterAll();

  Py_AtExit(&cleanup);

  
  Py_INCREF(&CorpusReaderType);
  PyModule_AddObject(m, "CorpusReader", (PyObject *) &CorpusReaderType);
  PyModule_AddObject(m, "EntryIterator", (PyObject *) &EntryIteratorType);
  PyModule_AddObject(m, "Entry", (PyObject *) &EntryType);
  PyModule_AddObject(m, "MarkerQuery", (PyObject *) &MarkerQueryType);

#if PY_MAJOR_VERSION >= 3
  return m;
#endif
}

