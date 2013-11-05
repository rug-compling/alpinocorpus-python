#include <stdexcept>
#include <string>
#include <cstring>

#include <Python.h>

#include <AlpinoCorpus/CorpusReader.hh>
#include <AlpinoCorpus/DbCorpusWriter.hh>
#include <AlpinoCorpus/CompactCorpusWriter.hh>

#include "alpinocorpus.h"
#include "CorpusReader.hh"
#include "CorpusWriter.hh"
#include "boost_wrap.hh"

static PyMethodDef CorpusWriter_methods[] = {
  {"write", (PyCFunction) CorpusWriter_write, METH_VARARGS, "write(name, contents)\n\n"
      "Write a single entry." },
  {"write_entries", (PyCFunction) CorpusWriter_write_entries, METH_VARARGS,
      "write_entries(corpus)\n\nWrite entries from a CorpusReader object. "
      "Returns number of entries written." },
  {NULL} // Sentinel
};

PyTypeObject CorpusWriterType = {
            PyVarObject_HEAD_INIT(NULL, 0)
            "alpinocorpus.CorpusWriter",              /* tp_name */
            sizeof(CorpusWriter),                     /* tp_basicsize */
            0,                                        /* tp_itemsize */
            (destructor)CorpusWriter_dealloc,         /* tp_dealloc */
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
            "CorpusWriter(filename, writertype='dact')\n\n"
            "... where filename is the file to be written to, and writertype "
            "may be 'dact' or 'compact'.",            /* tp_doc */
            0,                                        /* tp_traverse */
            0,                                        /* tp_clear */
            0,                                        /* tp_richcompare */
            0,                                        /* tp_weaklistoffset */
            0,                                        /* tp_iter */
            0,                                        /* tp_iternext */
            CorpusWriter_methods,                     /* tp_methods */
            0,                                        /* tp_members */
            0,                                        /* tp_getset */
            0,                                        /* tp_base */
            0,                                        /* tp_dict */
            0,                                        /* tp_descr_get */
            0,                                        /* tp_descr_set */
            0,                                        /* tp_dictoffset */
            0,                                        /* tp_init */
            0,                                        /* tp_alloc */
            CorpusWriter_new,                         /* tp_new */
        };

PyObject *CorpusWriter_new(PyTypeObject *type, PyObject *args,
  PyObject *kwds)
{
  static char const *kwlist[] = {"filename", "writertype", NULL};

  char *path, *writertype = NULL;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|s", (char **) kwlist,
        &path, &writertype))
    return NULL;

  CorpusWriter *self;
  self = (CorpusWriter *) type->tp_alloc(type, 0);
  try {
    if (self != NULL) {
      if (writertype == NULL || strcmp(writertype, "dact") == 0)
        self->writer = new alpinocorpus::DbCorpusWriter(path, true);
      else if (strcmp(writertype, "compact") == 0)
        self->writer = new alpinocorpus::CompactCorpusWriter(path);
      //else if (strcmp(writertype, "directory")
      //  self->writer = new DirectoryCorpusWriter(filename);
      else
        raise_exception("Trying to write to a corpus of unknown type");
    }
  } catch (std::runtime_error &e) {
    raise_exception(e.what());
    return NULL;
  }

  return (PyObject *) self;
}

void CorpusWriter_dealloc(CorpusWriter *self)
{
  delete self->writer;
  OB_TYPE(self)->tp_free(self);
}

PyObject *CorpusWriter_write(CorpusWriter *self, PyObject *args)
{
  char *name, *content;
  if (!PyArg_ParseTuple(args, "ss", &name, &content))
    return NULL;

  self->writer->write(name, content);

  return Py_BuildValue("i", 1);
}

PyObject *CorpusWriter_write_entries(CorpusWriter *self, PyObject *args)
{
  CorpusReader *corpus;
  if (!PyArg_ParseTuple(args, "O!", &CorpusReaderType, &corpus))
    return NULL;

  self->writer->write(*corpus->reader, false);

  return Py_BuildValue("i", corpus->reader->size());
}
