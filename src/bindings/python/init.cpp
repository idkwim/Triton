
#include <iostream>
#include <python2.7/Python.h>

#include "SymbolicEngine.h"
#include "PythonBindings.h"
#include "xPyFunc.h"

void initCallbackEnv(PyObject *);
void initFlagEnv(PyObject *);
void initOpcodeCategoryEnv(PyObject *);
void initOpcodeEnv(PyObject *);
void initOperandEnv(PyObject *);
void initRegEnv(PyObject *);
void initSyscallEnv(PyObject *);


void initBindings(void)
{
  Py_Initialize();

  PyObject *tritonModule = Py_InitModule("triton", tritonCallbacks);

  if (tritonModule == nullptr) {
    std::cerr << "Failed to initialize the triton bindings" << std::endl;
    PyErr_Print();
    exit(1);
  }

  PyObject *smt2libModule = Py_InitModule("smt2lib", smt2libCallbacks);

  if (smt2libModule == nullptr) {
    std::cerr << "Failed to initialize the smt2lib bindings" << std::endl;
    PyErr_Print();
    exit(1);
  }

  /* Create the IDREF class */
  PyObject *idRefClassName = xPyString_FromString("IDREF");
  PyObject *idRefClassDict = xPyDict_New();


  // REG ---------------------

  /* Create the IDREF.REG class */
  PyObject *idRegClassName = xPyString_FromString("REG");
  PyObject *idRegClassDict = xPyDict_New();

  /* Add registers ref into IDREF.REG class */
  initRegEnv(idRegClassDict);

  /* Create the REG class */
  PyObject *idRegClass = xPyClass_New(nullptr, idRegClassDict, idRegClassName);

  // REG ---------------------


  // FLAG ---------------------

  /* Create the IDREF.FLAG class */
  PyObject *idFlagClassName = xPyString_FromString("FLAG");
  PyObject *idFlagClassDict = xPyDict_New();

  /* Add flags ref into IDREF.FLAG class */
  initFlagEnv(idFlagClassDict);

  /* Create the FLAG class */
  PyObject *idFlagClass = xPyClass_New(nullptr, idFlagClassDict, idFlagClassName);

  // FLAG ---------------------


  // OPCODE ---------------------

  /* Create the IDREF.OPCODE class */
  PyObject *idOpcodeClassName = xPyString_FromString("OPCODE");
  PyObject *idOpcodeClassDict = xPyDict_New();

  /* Add registers ref into IDREF.OPCODE class */
  initOpcodeEnv(idOpcodeClassDict);

  /* Create the OPCODE class */
  PyObject *idOpcodeClass = xPyClass_New(nullptr, idOpcodeClassDict, idOpcodeClassName);

  // OPCODE ---------------------


  // OPCODE_CATEGORY ---------------------

  /* Create the IDREF.OPCODE_CATEGORY class */
  PyObject *idOpcodeCategoryClassName = xPyString_FromString("OPCODE_CATEGORY");
  PyObject *idOpcodeCategoryClassDict = xPyDict_New();

  /* Add registers ref into IDREF.OPCODE_CATEGORY class */
  initOpcodeCategoryEnv(idOpcodeCategoryClassDict);

  /* Create the OPCODE_CATEGORY class */
  PyObject *idOpcodeCategoryClass = xPyClass_New(nullptr, idOpcodeCategoryClassDict, idOpcodeCategoryClassName);

  // OPCODE_CATEGORY ---------------------


  // OPERAND ---------------------

  /* Create the IDREF.OPERAND class */
  PyObject *idOperandClassName = xPyString_FromString("OPERAND");
  PyObject *idOperandClassDict = xPyDict_New();

  /* Add registers ref into IDREF.OPERAND class */
  initOperandEnv(idOperandClassDict);

  /* Create the OPCODE class */
  PyObject *idOperandClass = xPyClass_New(nullptr, idOperandClassDict, idOperandClassName);

  // OPERAND ---------------------


  // CALLBACK ---------------------

  /* Create the IDREF.CALLBACK class */
  PyObject *idCallbackClassName = xPyString_FromString("CALLBACK");
  PyObject *idCallbackClassDict = xPyDict_New();

  /* Add registers ref into IDREF.CALLBACK class */
  initCallbackEnv(idCallbackClassDict);

  /* Create the CALLBACK class */
  PyObject *idCallbackClass = xPyClass_New(nullptr, idCallbackClassDict, idCallbackClassName);

  // CALLBACK ---------------------


  // SYSCALL ---------------------

  /* Create the IDREF.SYSCALL class */
  PyObject *idSyscallClassName = xPyString_FromString("SYSCALL");
  PyObject *idSyscallClassDict = xPyDict_New();

  /* Add registers ref into IDREF.SYSCALL class */
  initSyscallEnv(idSyscallClassDict);

  /* Create the SYSCALL class */
  PyObject *idSyscallClass = xPyClass_New(nullptr, idSyscallClassDict, idSyscallClassName);

  // SYSCALL ---------------------


  /* Add REG, FLAG, OPCODE, OPCODE_CATEGORY, OPERAND into IDREF */
  PyDict_SetItemString(idRefClassDict, "CALLBACK", idCallbackClass);
  PyDict_SetItemString(idRefClassDict, "FLAG", idFlagClass);
  PyDict_SetItemString(idRefClassDict, "OPCODE", idOpcodeClass);
  PyDict_SetItemString(idRefClassDict, "OPCODE_CATEGORY", idOpcodeCategoryClass);
  PyDict_SetItemString(idRefClassDict, "OPERAND", idOperandClass);
  PyDict_SetItemString(idRefClassDict, "REG", idRegClass);
  PyDict_SetItemString(idRefClassDict, "SYSCALL", idSyscallClass);

  /* Create the IDREF class */
  PyObject *idRefClass = xPyClass_New(nullptr, idRefClassDict, idRefClassName);


  /* add all classes and constants into the triton module */
  PyModule_AddObject(tritonModule, "IDREF", idRefClass);
  PyModule_AddObject(tritonModule, "UNSET", Py_BuildValue("k", UNSET)); // Py_BuildValue for unsigned long
}


bool execBindings(const char *fileName)
{
  FILE *fd = fopen(fileName, "r");
  if (fd == nullptr) {
    perror("fopen");
    return false;
  }
  PyRun_SimpleFile(fd, fileName);
  fclose(fd);
  return true;
}

