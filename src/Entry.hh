#ifndef PYTHON_ENTRY_HH
#define PYTHON_ENTRY_HH

#include <Python.h>

#include <AlpinoCorpus/Entry.hh>

#include "EntryIterator.hh"

// This object is what iteration over entries yields. We need to hold
// the iterator for the contents() method.
struct Entry {
  PyObject_HEAD
  
  PyObject *name;
  PyObject *shortName;
  PyObject *contents;
};

extern PyTypeObject EntryType;

PyObject *Entry_new(alpinocorpus::Entry const &entry);
void Entry_dealloc(Entry *self);
PyObject *Entry_contents(Entry *self);
PyObject *Entry_name(Entry *self);
PyObject *Entry_shortName(Entry *self);

#endif // PYTHON_ENTRY_HH
