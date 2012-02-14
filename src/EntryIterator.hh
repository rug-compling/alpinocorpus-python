#ifndef PYTHON_ENTRY_ITERATOR_HH
#define PYTHON_ENTRY_ITERATOR_HH

#include <Python.h>

#include <AlpinoCorpus/CorpusReader.hh>

#include "CorpusReader.hh"

namespace boost {
  class thread;
}

struct EntryIterator {
  PyObject_HEAD
  CorpusReader *reader;

  // This is a bit ugly, but we have to allocate the iterator separately.
  // Python lives in C-land, and tp_dealloc will not call the iterator
  // destructor. By managing the memory ourselves, we can use the delete
  // operator.
  alpinocorpus::CorpusReader::EntryIterator *iter;
  boost::thread *interruptThread;
};

extern PyTypeObject EntryIteratorType;

void EntryIterator_dealloc(EntryIterator *self);
PyObject *EntryIterator_iter(PyObject *self);
PyObject *EntryIterator_iternext(PyObject *self);

#endif // PYTHON_ENTRY_ITERATOR_HH
