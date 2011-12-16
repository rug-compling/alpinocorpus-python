#include "MarkerQuery.hh"

static PyMethodDef MarkerQuery_methods[] = {
  {NULL} // Sentinel
};

PyTypeObject MarkerQueryType = {
            PyObject_HEAD_INIT(NULL)
            0,                                        /* ob_size */
            "alpinocorpus.MarkerQuery",               /* tp_name */
            sizeof(MarkerQuery),                      /* tp_basicsize */
            0,                                        /* tp_itemsize */
            (destructor)MarkerQuery_dealloc,          /* tp_dealloc */
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
            "MarkerQuert objects",                    /* tp_doc */
            0,                                        /* tp_traverse */
            0,                                        /* tp_clear */
            0,                                        /* tp_richcompare */
            0,                                        /* tp_weaklistoffset */
            0,                                        /* tp_iter */
            0,                                        /* tp_iternext */
            MarkerQuery_methods,                      /* tp_methods */
            0,                                        /* tp_members */
            0,                                        /* tp_getset */
            0,                                        /* tp_base */
            0,                                        /* tp_dict */
            0,                                        /* tp_descr_get */
            0,                                        /* tp_descr_set */
            0,                                        /* tp_dictoffset */
            0,                                        /* tp_init */
            0,                                        /* tp_alloc */
            MarkerQuery_new,                          /* tp_new */
        };

PyObject *MarkerQuery_new(PyTypeObject *type, PyObject *args,
  PyObject *kwds)
{
  char *query, *attr, *value;

  if (!PyArg_ParseTuple(args, "sss", &query, &attr, &value))
    return NULL;

  MarkerQuery *self;
  if ((self = reinterpret_cast<MarkerQuery *>(type->tp_alloc(type, 0))) == NULL)
    return NULL;

  self->query = new std::string(query);
  self->attr = new std::string(attr);
  self->value = new std::string(value);

  return reinterpret_cast<PyObject *>(self);
}

void MarkerQuery_dealloc(MarkerQuery *self)
{
  delete self->query;
  delete self->attr;
  delete self->value;

  self->ob_type->tp_free(self);
}