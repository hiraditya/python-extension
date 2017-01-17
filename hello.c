// https://www.tutorialspoint.com/python/python_further_extensions.htm

#include <Python.h>

static PyObject* helloworld(PyObject* self)
{
  return Py_BuildValue("s", "Hello world");
}

static PyObject* helloworldname(PyObject* self)
{
  return Py_BuildValue("i", 100);
}

static PyObject *SpamError;

static PyObject *
spam_system(PyObject *self, PyObject *args)
{
  const char *command;
  int sts;

  if (!PyArg_ParseTuple(args, "s", &command))
    return NULL;

  // Execute the command passed from python.
  sts = system(command);
  if (sts < 0) {
    PyErr_SetString(SpamError, "System command failed");
    return NULL;
  }

  return PyLong_FromLong(sts);
}

static char helloworld_docs[] =
  "helloworld( ): Prints hello world.\n";

static char spam_docs[] =
  "spam_system( ): Executes the command on shell.\n";

/* Syntax of PyMethodDef
struct PyMethodDef {
  char *ml_name;
  PyCFunction ml_meth;
  int ml_flags;
  char *ml_doc;
};
*/

static PyMethodDef helloworld_funcs[] = {
  { "helloworld",
    (PyCFunction)helloworld,
    METH_NOARGS,
    helloworld_docs
  },

  { "helloworld_name", // Function name referred by python interpreter
    (PyCFunction)helloworldname, // The C function
    METH_NOARGS,
    helloworld_docs
  },

  { "spam_system",
    (PyCFunction)spam_system,
    METH_VARARGS,
    spam_docs
  },

  { NULL, NULL, 0, NULL } // Sentinel
};

void inithelloworld(void)
{
  PyObject *m;
  m = Py_InitModule3("helloworld", helloworld_funcs,
      "Extension module example!");

  if (m == NULL)
      return;
  SpamError = PyErr_NewException("spam.error", NULL, NULL);
  Py_INCREF(SpamError);
  PyModule_AddObject(m, "error", SpamError);
}

