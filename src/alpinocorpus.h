#ifndef PYTHON_ALPINOCORPUS_HH
#define PYTHON_ALPINOCORPUS_HH

#include <Python.h>

void raise_exception(char const *msg);

// PyVarObject_HEAD_INIT does not exist in Python versions prior to 2.6.
#ifndef PyVarObject_HEAD_INIT
    #define PyVarObject_HEAD_INIT(type, size) \
          PyObject_HEAD_INIT(type) size,
#endif

#if PY_MAJOR_VERSION >= 3
#define OB_TYPE(o) \
  o->ob_base.ob_type
#else
#define OB_TYPE(o) \
  o->ob_type
#endif

#if PY_MAJOR_VERSION >= 3
#define MODULE_RETURN(o)\
  return o;
#else
#define MODULE_RETURN(o) \
  return;
#endif

#endif // PYTHON_ALPINOCORPUS_HH
