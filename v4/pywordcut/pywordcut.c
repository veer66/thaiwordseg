#include<Python.h>
#include"structmember.h"
#include<wordcut/wordcut.h>

typedef struct {
    PyObject_HEAD
    Wordcut wordcut;
} PyWordcut;

static void
py_wordcut_dealloc(PyWordcut *self)
{
    wordcut_close(&(self->wordcut));
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject*
py_wordcut_new(PyTypeObject *type,PyObject *args,PyObject *kword)
{
    PyWordcut *self;
    self = (PyWordcut *) type->tp_alloc(type,0);
    return (PyObject *)self;   
}

static int
py_wordcut_init(PyWordcut *self,PyObject *args,PyObject* kword)
{
    static char *kword_list[]={"filename"};
    char *filename;
    if(!PyArg_ParseTupleAndKeywords(args,kword,"s",kword_list,&filename)) {
        return -1;
    }
    if (filename!=NULL) {
        if(wordcut_init(&(self->wordcut),filename)!=0) {
            return -1;
        }
        return 0;
    } else {
        return -1;
    }                                    
}

static PyObject *
py_wordcut_cut_ret_str_list(PyWordcut* self,PyObject *args)
{
#define MAXBUF 4096
    char *str;
    char buffer[MAXBUF];    
    WordcutResult result;
    PyObject *list,*pystr;
    int i;
    
    if(!PyArg_ParseTuple(args,"s",&str)) {
        return NULL;
    }
    if (str==NULL) return NULL;
    wordcut_cut(&self->wordcut,str,&result);
    list=PyList_New(result.count);
    for(i=0;i<result.count;i++) {
        /* TODO: check result.offset[i] + 1 < MAXBUF */
        memcpy(buffer,
               result.str+result.start[i],
               result.offset[i]);
        buffer[result.offset[i]]='\0';
        pystr=PyString_FromString(buffer);
        Py_INCREF(pystr);        
        if (PyList_SetItem(list,i,pystr)!=0) return NULL;
    }
    Py_INCREF(list);
    return list;
    
}


static PyMethodDef PyWordcut_methods[] = {
    {"cut_s",(PyCFunction)py_wordcut_cut_ret_str_list,METH_VARARGS},
    {NULL}  /* Sentinel */
};


static PyGetSetDef PyWordcut_getseters[] = {
    {NULL}  /* Sentinel */
};

static PyMemberDef PyWordcut_members[] = {
    {NULL}  /* Sentinel */
};





static PyTypeObject PyWordcutType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "wordcut.Wordcut",         /*tp_name*/
    sizeof(PyWordcut),           /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)py_wordcut_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "PyWordcut objects",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    PyWordcut_methods,             /* tp_methods */
    PyWordcut_members,             /* tp_members */
    PyWordcut_getseters,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)py_wordcut_init,      /* tp_init */
    0,                         /* tp_alloc */
    py_wordcut_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initwordcut(void) 
{
    PyObject* m;

    if (PyType_Ready(&PyWordcutType) < 0)
        return;

    m = Py_InitModule3("wordcut", module_methods,
                       "Wordcut module");

    if (m == NULL)
      return;

    Py_INCREF(&PyWordcutType);
    PyModule_AddObject(m, "Wordcut", (PyObject *)&PyWordcutType);
    
}
