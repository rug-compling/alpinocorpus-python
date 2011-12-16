#ifndef PYTHON_ENTRY_HH
#define PYTHON_ENTRY_HH

#include <Python.h>

#include "EntryIterator.hh"

// This object is what iteration over entries yields. We need to hold
// the iterator for the contents() method.
struct Entry {
  PyObject_HEAD
  
  PyObject *name;
  PyObject *contents;
};

extern PyTypeObject EntryType;

PyObject *Entry_new(EntryIterator *iter);
void Entry_dealloc(Entry *self);
PyObject *Entry_contents(Entry *self);
PyObject *Entry_name(Entry *self);

#endif // PYTHON_ENTRY_HH
