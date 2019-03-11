#include <python3.6/Python.h>

int * _optimize(int (func)(int*), int learning_rate, int steps, int* init_theta, int dx, int num_theta){
    int* theta = init_theta;
    for(int i = 0; i < steps; i ++){
        int * partials = malloc(sizeof(int) * num_theta);
        for (int t=0; t < num_theta; t++){
            int * theta_dx = malloc(sizeof(int) * num_theta);
            for( int x = 0; x < num_theta; x++){
                if(t == x){
                    theta_dx[x] = theta[x] + dx;
                }
                else{
                    theta_dx[x] = theta[x];
                }
            }
            partials[t] = func(theta) - func(theta) / dx;
        }
        for(int k = 0; k < num_theta; k++){
            theta[k] -= learning_rate * partials[k];
        }
        if (i % 50 == 0){
            printf("Step: {%d} Cost {%d}", i, func(theta));
        }
    return theta;
    }
}

static PyObject * optimize(PyObject* self, PyObject* args){
    /*int* theta;
    int learning_rate;
    int steps;
    int* init_theta;
    int dx;
    int num_theta;*/
    PyObject* func;
    char* input2;
    char* input;

    printf("In the function!\n");
    if (!PyArg_ParseTuple(args, "ssO", &input, &input2, &func))
        return NULL;
    PyObject* result = PyObject_CallObject(func, args);
    /*const char* s = PyString_AsString(result);
    printf("%s", s);*/
    PyObject* repr = PyObject_Repr(result);
    PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
    const char *bytes = PyBytes_AS_STRING(str);

    printf("REPR: %s\n", bytes);

    Py_XDECREF(repr);
    Py_XDECREF(str);

    return Py_BuildValue("s", bytes);
}

static char optimize_docs[] =
    "usage: optimize()\n";


static PyMethodDef module_methods[] = {
    {"optimize", (PyCFunction) optimize, 
     METH_VARARGS, optimize_docs},
    {NULL}
};


static struct PyModuleDef gradient_descent =
{
    PyModuleDef_HEAD_INIT,
    "gradient_descent", 
    "usage: gradient_descent.optimize\n", 
    -1,   
    module_methods
};


PyMODINIT_FUNC PyInit_gradient_descent(void)
{
    return PyModule_Create(&gradient_descent);
}