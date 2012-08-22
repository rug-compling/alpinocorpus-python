#include <AlpinoCorpus/Entry.hh>
#include <AlpinoCorpus/Error.hh>

#include "alpinocorpus.h"
#include "CorpusReader.hh"
#include "Entry.hh"
#include "EntryIterator.hh"
#include "boost_wrap.hh"


namespace ac = alpinocorpus;

static PyMethodDef EntryIterator_methods[] = {
  {NULL} // Sentinel
};

PyTypeObject EntryIteratorType = {
            PyVarObject_HEAD_INIT(NULL, 0)
            "alpinocorpus.EntryIterator",             /* tp_name */
            sizeof(EntryIterator),                    /* tp_basicsize */
            0,                                        /* tp_itemsize */
            (destructor)EntryIterator_dealloc,        /* tp_dealloc */
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
            "EntryIterator objects",                  /* tp_doc */
            0,                                        /* tp_traverse */
            0,                                        /* tp_clear */
            0,                                        /* tp_richcompare */
            0,                                        /* tp_weaklistoffset */
            EntryIterator_iter,                       /* tp_iter */
            EntryIterator_iternext,                   /* tp_iternext */
            EntryIterator_methods,                    /* tp_methods */
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


void EntryIterator_dealloc(EntryIterator *self)
{
  // We clean up the iterator before decrementing the reference count of
  // the reader. If we hold the last reference, we'd deallocate the corpus
  // reader, while the iterator could still hold resources.
  CorpusReader *reader = self->reader;

  if (self->interruptThread != 0) {
    interruptBoostThread(self->interruptThread);
    deleteBoostThread(self->interruptThread);
  }

  delete self->iter;
  OB_TYPE(self)->tp_free(self);

  Py_DECREF(reader);
}

PyObject *EntryIterator_iter(PyObject *self)
{
  Py_INCREF(self);
  return self;
}

PyObject *EntryIterator_iternext(PyObject *self)
{
  EntryIterator *entryIterator = reinterpret_cast<EntryIterator *>(self);

  bool notAtEnd;
  Py_BEGIN_ALLOW_THREADS
  try {
    notAtEnd = entryIterator->iter->hasNext();
  } catch (ac::IterationInterrupted &e) {
    Py_BLOCK_THREADS
    PyErr_SetNone(PyExc_GeneratorExit);
    return NULL;
  } catch (std::runtime_error &e) {
    Py_BLOCK_THREADS
    raise_exception(e.what());
    return NULL;
  }
  Py_END_ALLOW_THREADS

  if (notAtEnd) {
    ac::Entry e;

    Py_BEGIN_ALLOW_THREADS
    try {
      e = entryIterator->iter->next(*entryIterator->reader->reader);
    } catch (ac::IterationInterrupted &e) {
      Py_BLOCK_THREADS
      // XXX - throw a specific exception here
      PyErr_SetNone(PyExc_GeneratorExit);
      return NULL;
    } catch (std::runtime_error &e) {
      Py_BLOCK_THREADS
      raise_exception(e.what());
      return NULL;
    }
    Py_END_ALLOW_THREADS

    PyObject *entry = Entry_new(e);

    return entry;
  } else {
    PyErr_SetNone(PyExc_StopIteration);
    return NULL;
  }
}
