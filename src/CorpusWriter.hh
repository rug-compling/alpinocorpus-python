#ifndef PYTHON_CORPUSWRITER_HH
#define PYTHON_CORPUSWRITER_HH

#include <Python.h>

#include <AlpinoCorpus/CorpusWriter.hh>

struct CorpusWriter {
  PyObject_HEAD
  alpinocorpus::CorpusWriter *writer;
};

extern PyTypeObject CorpusWriterType;

PyObject *CorpusWriter_new(PyTypeObject *type, PyObject *args,
  PyObject *kwds);
void CorpusWriter_dealloc(CorpusWriter *self);
PyObject *CorpusWriter_write(CorpusWriter *self, PyObject *args);
PyObject *CorpusWriter_write_entries(CorpusWriter *self, PyObject *args);

#endif // PYTHON_CORPUSWRITER_HH
