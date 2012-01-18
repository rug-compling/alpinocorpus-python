#ifndef PYTHON_CORPUSREADER_HH
#define PYTHON_CORPUSREADER_HH

#include <Python.h>

#include <AlpinoCorpus/CorpusReader.hh>

struct CorpusReader {
  PyObject_HEAD
  alpinocorpus::CorpusReader *reader;
};

extern PyTypeObject CorpusReaderType;

PyObject *CorpusReader_new(PyTypeObject *type, PyObject *args,
  PyObject *kwds);
void CorpusReader_dealloc(CorpusReader *self);
PyObject *CorpusReader_entries(CorpusReader *self);
PyObject *CorpusReader_query(CorpusReader *self, PyObject *args);
PyObject *CorpusReader_read(CorpusReader *self, PyObject *args);
PyObject *CorpusReader_readMarkQueries(CorpusReader *self, PyObject *args);
PyObject *CorpusReader_size(CorpusReader *self);
PyObject *CorpusReader_validQuery(CorpusReader *self, PyObject *args);

#endif // PYTHON_CORPUSREADER_HH
