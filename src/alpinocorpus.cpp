#include <Python.h>

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

PyMODINIT_FUNC initalpinocorpus(void)
{
  static int initialized = 0;

  PyObject *m;

  if (PyType_Ready(&CorpusReaderType) < 0)
    return;
  if (PyType_Ready(&EntryIteratorType) < 0)
    return;
  if (PyType_Ready(&EntryType) < 0)
    return;
  if (PyType_Ready(&MarkerQueryType) < 0)
    return;

  if (initialized != 0)
    return;
  
  m = Py_InitModule("alpinocorpus", AlpinoCorpusMethods);
  if (m == NULL)
    return;
  
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
}

