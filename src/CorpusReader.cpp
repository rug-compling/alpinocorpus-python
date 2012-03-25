#include <stdexcept>
#include <string>
#include <cstring>

#include <Python.h>

#include <AlpinoCorpus/CorpusReader.hh>
#include <AlpinoCorpus/CorpusReaderFactory.hh>

#include "alpinocorpus.h"
#include "CorpusReader.hh"
#include "EntryIterator.hh"
#include "MarkerQuery.hh"
#include "boost_wrap.hh"

static PyMethodDef CorpusReader_methods[] = {
  {"entries", (PyCFunction) CorpusReader_entries, METH_NOARGS, "Entries" },
  {"entriesWithStylesheet", (PyCFunction) CorpusReader_entriesWithStylesheet,
    METH_VARARGS, "Iterate over entries, transforming them with a stylesheet" },
  {"query", (PyCFunction) CorpusReader_query, METH_VARARGS, "Execute a query" },
  {"queryWithStylesheet", (PyCFunction) CorpusReader_queryWithStylesheet, METH_VARARGS, "Execute a query and transform the results with a stylesheet" },
  {"read", (PyCFunction) CorpusReader_read, METH_VARARGS, "Read entry" },
  {"readMarkQueries", (PyCFunction) CorpusReader_readMarkQueries, METH_VARARGS, "Read entry, marking queries" },
  {"size", (PyCFunction) CorpusReader_size, METH_NOARGS, "Get the number of corpus entries" },
  {"validQuery", (PyCFunction) CorpusReader_validQuery, METH_VARARGS, "Validate a query" },
  {NULL} // Sentinel
};

PyTypeObject CorpusReaderType = {
            PyVarObject_HEAD_INIT(NULL, 0)
            "alpinocorpus.CorpusReader",              /* tp_name */
            sizeof(CorpusReader),                     /* tp_basicsize */
            0,                                        /* tp_itemsize */
            (destructor)CorpusReader_dealloc,         /* tp_dealloc */
            0,                                        /* tp_print */
            0,                                        /* tp_getattr */
            0,                                        /* tp_setattr */
            0,                                        /* tp_compare */
            0,                                        /* tp_repr */
            0,                                        /* tp_as_number */
            0,                                        /* tp_as_sequence */
            0,                                        /* tp_as_mapping */
            0,                                        /* tp_hash */
            0,                                        /* tp_call */
            0,                                        /* tp_str */
            0,                                        /* tp_getattro */
            0,                                        /* tp_setattro */
            0,                                        /* tp_as_buffer */
            Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
            "CorpusReader objects",                   /* tp_doc */
            0,                                        /* tp_traverse */
            0,                                        /* tp_clear */
            0,                                        /* tp_richcompare */
            0,                                        /* tp_weaklistoffset */
            0,                                        /* tp_iter */
            0,                                        /* tp_iternext */
            CorpusReader_methods,                     /* tp_methods */
            0,                                        /* tp_members */
            0,                                        /* tp_getset */
            0,                                        /* tp_base */
            0,                                        /* tp_dict */
            0,                                        /* tp_descr_get */
            0,                                        /* tp_descr_set */
            0,                                        /* tp_dictoffset */
            0,                                        /* tp_init */
            0,                                        /* tp_alloc */
            CorpusReader_new,                         /* tp_new */
        };

std::list<alpinocorpus::CorpusReader::MarkerQuery> parseMarkerQueries(PyObject *markerList)
{
  std::list<alpinocorpus::CorpusReader::MarkerQuery> markerQueries;

  for (int i = 0; i < PyList_Size(markerList); ++i) {
    PyObject *entry = PyList_GetItem(markerList, (Py_ssize_t) i);
    if (entry->ob_type != &MarkerQueryType) {
      raise_exception("Marker list contains non-EntryMarker entries.");
      break;
    }

    MarkerQuery *marker = reinterpret_cast<MarkerQuery *>(entry);
    markerQueries.push_back(alpinocorpus::CorpusReader::MarkerQuery(
      *marker->query, *marker->attr, *marker->value));
  }

  return markerQueries;
}

void prepareTimeout(EntryIterator *iter, int timeout)
{
  if (timeout != -1) {
    InterruptIter interruptFun(iter, timeout);
    iter->interruptThread = createBoostThread(interruptFun);
  }
  else
    iter->interruptThread = 0;
}

PyObject *CorpusReader_new(PyTypeObject *type, PyObject *args,
  PyObject *kwds)
{
  char *path;
  if (!PyArg_ParseTuple(args, "s", &path))
    return NULL;

  CorpusReader *self;
  self = (CorpusReader *) type->tp_alloc(type, 0);
  try {
    if (self != NULL) {
      if (path[strlen(path) - 1] == '/')
	self->reader = alpinocorpus::CorpusReaderFactory::openRecursive(path);
      else
	self->reader = alpinocorpus::CorpusReaderFactory::open(path);
    }
  } catch (std::runtime_error &e) {
    raise_exception(e.what());
    return NULL;
  }

  return (PyObject *) self;
}

void CorpusReader_dealloc(CorpusReader *self)
{
  delete self->reader;
  OB_TYPE(self)->tp_free(self);
}

PyObject *CorpusReader_read(CorpusReader *self, PyObject *args)
{
  char *entry;
  if (!PyArg_ParseTuple(args, "s", &entry))
    return NULL;

  std::string data;
  Py_BEGIN_ALLOW_THREADS
  try {
    data = self->reader->read(entry);
  } catch (std::runtime_error &e) {
    Py_BLOCK_THREADS
    raise_exception(e.what());
    return NULL;
  }
  Py_END_ALLOW_THREADS

  return Py_BuildValue("s#", data.c_str(), data.size());
}

PyObject *CorpusReader_readMarkQueries(CorpusReader *self, PyObject *args)
{
  char *entry;
  PyObject *markerList;
  if (!PyArg_ParseTuple(args, "sO!", &entry, &PyList_Type, &markerList))
    return NULL;

  std::list<alpinocorpus::CorpusReader::MarkerQuery> markerQueries = parseMarkerQueries(markerList);

  std::string data;
  Py_BEGIN_ALLOW_THREADS
  try {
    data = self->reader->read(entry, markerQueries);
  } catch (std::runtime_error &e) {
    Py_BLOCK_THREADS
    raise_exception(e.what());
    return NULL;
  }
  Py_END_ALLOW_THREADS

  return Py_BuildValue("s#", data.c_str(), data.size());
}

PyObject *CorpusReader_entries(CorpusReader *self)
{
  EntryIterator *iter;
  iter = (EntryIterator *) EntryIteratorType.tp_alloc(&EntryIteratorType, 0);

  try {
    if (iter != NULL) {
      iter->reader = self;
      iter->iter = new alpinocorpus::CorpusReader::EntryIterator(self->reader->begin());

      // Ensure the reader is not deallocated, since we need it.
      Py_INCREF(iter->reader);
    }
  } catch (std::runtime_error &e) {
    raise_exception(e.what());
    return NULL;
  }

  return (PyObject *) iter;
}

PyObject *CorpusReader_entriesWithStylesheet(CorpusReader *self, PyObject *args)
{
  char *stylesheet;
  PyObject *markerList;
  int timeout = -1;
  if (!PyArg_ParseTuple(args, "sO!|i", &stylesheet, &PyList_Type, &markerList,
        &timeout))
    return NULL;

  std::list<alpinocorpus::CorpusReader::MarkerQuery> markerQueries = parseMarkerQueries(markerList);

  EntryIterator *iter;
  iter = (EntryIterator *) EntryIteratorType.tp_alloc(&EntryIteratorType, 0);

  try {
    if (iter != NULL) {
      iter->reader = self;
      iter->iter = new alpinocorpus::CorpusReader::EntryIterator(
        self->reader->beginWithStylesheet(stylesheet, markerQueries));

      // Ensure the reader is not deallocated, since we need it.
      Py_INCREF(iter->reader);
    }
  } catch (std::runtime_error &e) {
    raise_exception(e.what());
    return NULL;
  }

  prepareTimeout(iter, timeout);

  return (PyObject *) iter;
}

PyObject *CorpusReader_query(CorpusReader *self, PyObject *args)
{
  char *query;
  int timeout = -1;
  if (!PyArg_ParseTuple(args, "s|i", &query, &timeout))
    return NULL;

  EntryIterator *iter;
  iter = (EntryIterator *) EntryIteratorType.tp_alloc(&EntryIteratorType, 0);
  try {
    if (iter != NULL) {
      iter->reader = self;
      iter->iter = new alpinocorpus::CorpusReader::EntryIterator(self->reader->query(alpinocorpus::CorpusReader::XPATH, query));

      // Ensure the reader is not deallocated, since we need it.
      Py_INCREF(iter->reader);
    }
  } catch (std::runtime_error &e) {
    EntryIteratorType.tp_free(iter);
    raise_exception(e.what());
    return NULL;
  }

  prepareTimeout(iter, timeout);

  return (PyObject *) iter;
}

PyObject *CorpusReader_queryWithStylesheet(CorpusReader *self, PyObject *args)
{
  char *query, *stylesheet;
  PyObject *markerList;
  int timeout = -1;
  if (!PyArg_ParseTuple(args, "ssO!|i", &query, &stylesheet, &PyList_Type,
        &markerList, &timeout))
    return NULL;

  std::list<alpinocorpus::CorpusReader::MarkerQuery> markerQueries = parseMarkerQueries(markerList);
  
  EntryIterator *iter;
  iter = (EntryIterator *) EntryIteratorType.tp_alloc(&EntryIteratorType, 0);

  try {
    if (iter != NULL) {
      iter->reader = self;
      iter->iter = new alpinocorpus::CorpusReader::EntryIterator(self->reader->queryWithStylesheet(alpinocorpus::CorpusReader::XPATH, query, stylesheet, markerQueries));

      // Ensure the reader is not deallocated, since we need it.
      Py_INCREF(iter->reader);
    }
  } catch (std::runtime_error &e) {
    EntryIteratorType.tp_free(iter);
    raise_exception(e.what());
    return NULL;
  }

  prepareTimeout(iter, timeout);

  return (PyObject *) iter;
}

PyObject *CorpusReader_size(CorpusReader *self)
{
  size_t entries;
  Py_BEGIN_ALLOW_THREADS
  try {
    entries = self->reader->size();
  } catch (std::runtime_error &e) {
    Py_BLOCK_THREADS
    raise_exception(e.what());
    return NULL;
  }
  Py_END_ALLOW_THREADS

  return PyLong_FromSize_t(entries);
}

PyObject *CorpusReader_validQuery(CorpusReader *self, PyObject *args)
{
  char *query;
  if (!PyArg_ParseTuple(args, "s", &query))
    return NULL;

  bool valid;
  Py_BEGIN_ALLOW_THREADS
  try {
    valid = self->reader->isValidQuery(alpinocorpus::CorpusReader::XPATH, false,
        query);
  } catch (std::runtime_error &e) {
      Py_BLOCK_THREADS
      raise_exception(e.what());
      return NULL;
  }
  Py_END_ALLOW_THREADS

  if (valid)
    return Py_True;
  else
    return Py_False;
}
