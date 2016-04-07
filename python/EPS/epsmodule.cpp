#include <Python.h>
#include <stdint.h>
#include "../../arduino/libraries/ESP/ESP.h"

static PyObject *
eps_pack_gps(PyObject *self, PyObject *args){
  long lat,lon;
  if (!PyArg_ParseTuple(args,"l",&lat)){
    return NULL;
  }
  else{
    // pack message and return the string... somehow
    uint8_t*byt[256];
    *byt = 127;
    *(byt+1) = 25;
    return Py_BuildValue("H",byt[0]);
  }
};

static PyMethodDef EpsMethods[] = {
  {"pack_gps",  eps_pack_gps, METH_VARARGS,
     "Pack a GPS message."},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initeps(void)
{
    (void) Py_InitModule("eps", EpsMethods);
};
