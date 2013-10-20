#include <stdexcept>
#include <string>

#include <Python.h>

#include <AlpinoCorpus/CorpusReader.hh>
#include <AlpinoCorpus/Entry.hh>

#include "alpinocorpus.h"
#include "Entry.hh"

namespace ac = alpinocorpus;

static PyMethodDef Entry_methods[] = {
  {"name",      (PyCFunction) Entry_name,      METH_NOARGS, "Entry name" },
  {"shortName", (PyCFunction) Entry_shortName, METH_NOARGS, "Entry name (short)" },
  {"contents",  (PyCFunction) Entry_contents,  METH_NOARGS, "Entry contents" },
  {NULL} // Sentinel
};

PyTypeObject EntryType = {
            PyVarObject_HEAD_INIT(NULL, 0)
            "alpinocorpus.Entry",                     /* tp_name */
            sizeof(Entry),                            /* tp_basicsize */
            0,                                        /* tp_itemsize */
            (destructor)Entry_dealloc,                /* tp_dealloc */
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
            "Entry objects",                          /* tp_doc */
            0,                                        /* tp_traverse */
            0,                                        /* tp_clear */
            0,                                        /* tp_richcompare */
            0,                                        /* tp_weaklistoffset */
            0,                                        /* tp_iter */
            0,                                        /* tp_iternext */
            Entry_methods,                            /* tp_methods */
            0,                                        /* tp_members */
            0,                                        /* tp_getset */
            0,                                        /* tp_base */
            0,                                        /* tp_dict */
            0,                                        /* tp_descr_get */
            0,                                        /* tp_descr_set */
            0,                                        /* tp_dictoffset */
            0,                                        /* tp_init */
            0,                                        /* tp_alloc */
            0,                                        /* tp_new */
        };

PyObject *Entry_new(ac::Entry const &entry)
{
  Entry *pyEntry = reinterpret_cast<Entry *>(EntryType.tp_alloc(&EntryType, 0));

  pyEntry->name = Py_BuildValue("s#", entry.name().c_str(), entry.name().size());

  std::string shortName = entry.shortName();
  pyEntry->shortName = Py_BuildValue("s#", shortName.c_str(), shortName.size());

  // It's a shame that we have to copy the contents, but EntryIterators
  // are input iterators. So, we cannot keep an iterator pointed at a
  // previous entry to retrieve the contents at any time.
  pyEntry->contents = Py_BuildValue("s#", entry.contents().c_str(),
      entry.contents().size());

  return reinterpret_cast<PyObject *>(pyEntry);
}

void Entry_dealloc(Entry *self)
{
  Py_DECREF(self->name);
  Py_DECREF(self->shortName);
  Py_DECREF(self->contents);

  OB_TYPE(self)->tp_free(self);
}

PyObject *Entry_contents(Entry *self)
{
  Py_INCREF(self->contents);
  return self->contents;
}

PyObject *Entry_name(Entry *self)
{
  Py_INCREF(self->name);
  return self->name;
}

PyObject *Entry_shortName(Entry *self)
{
  Py_INCREF(self->shortName);
  return self->shortName;
}
