#include "lcms2.h"
#include <Python.h>

static char module_docstring[] = "make little CMS 2 available for python3";
static char single_RGB_DBL_to_CMYK_DBL_docstring[] = "convert single RGB value to CMYK";
static char single_CMYK_DBL_to_Lab_DBL_docstring[] = "convert single CMYK value to Lab";
static char single_Lab_DBL_to_CMYK_DBL_docstring[] = "convert single Lab value to CMYK";

// declaration of functions
static PyObject *pylcms2_single_RGB_DBL_to_CMYK_DBL(PyObject *self, PyObject *args);
static PyObject *pylcms2_single_CMYK_DBL_to_Lab_DBL(PyObject *self, PyObject *args);
static PyObject *pylcms2_single_Lab_DBL_to_CMYK_DBL(PyObject *self, PyObject *args);

// declaration of python-module methods
static PyMethodDef module_methods[] = {
    {"single_RGB_DBL_to_CMYK_DBL", pylcms2_single_RGB_DBL_to_CMYK_DBL, METH_VARARGS, single_RGB_DBL_to_CMYK_DBL_docstring},
    {"single_CMYK_DBL_to_Lab_DBL", pylcms2_single_CMYK_DBL_to_Lab_DBL, METH_VARARGS, single_CMYK_DBL_to_Lab_DBL_docstring},
    {"single_Lab_DBL_to_CMYK_DBL", pylcms2_single_Lab_DBL_to_CMYK_DBL, METH_VARARGS, single_Lab_DBL_to_CMYK_DBL_docstring},
    {NULL, NULL, 0, NULL}
};

// declaration of python-module
static struct PyModuleDef pylcms_module = {
    PyModuleDef_HEAD_INIT,
    "pylcms2",            /* name of module */
    module_docstring,     /* module documentation, may be NULL */
    -1,                   /* size of per-interpreter state of the module,
                             or -1 if the module keeps state in global variables. */
    module_methods
};

// init python module
PyMODINIT_FUNC PyInit_pylcms2(void)
{
    /* Load `numpy` functionality. */
//    import_array();

    PyObject * m = PyModule_Create(&pylcms_module);

//    define INTENT constants
    PyModule_AddIntConstant(m, "INTENT_PERCEPTUAL", INTENT_PERCEPTUAL);
    PyModule_AddIntConstant(m, "INTENT_RELATIVE_COLORIMETRIC", INTENT_RELATIVE_COLORIMETRIC);
    PyModule_AddIntConstant(m, "INTENT_SATURATION", INTENT_SATURATION);
    PyModule_AddIntConstant(m, "INTENT_ABSOLUTE_COLORIMETRIC", INTENT_ABSOLUTE_COLORIMETRIC);

//    define dwFlags
    PyModule_AddIntConstant(m, "cmsFLAGS_NOCACHE", cmsFLAGS_NOCACHE);
    PyModule_AddIntConstant(m, "cmsFLAGS_NOOPTIMIZE", cmsFLAGS_NOOPTIMIZE);
    PyModule_AddIntConstant(m, "cmsFLAGS_NULLTRANSFORM", cmsFLAGS_NULLTRANSFORM);
    PyModule_AddIntConstant(m, "cmsFLAGS_GAMUTCHECK", cmsFLAGS_GAMUTCHECK);
    PyModule_AddIntConstant(m, "cmsFLAGS_SOFTPROOFING", cmsFLAGS_SOFTPROOFING);
    PyModule_AddIntConstant(m, "cmsFLAGS_BLACKPOINTCOMPENSATION", cmsFLAGS_BLACKPOINTCOMPENSATION);
    PyModule_AddIntConstant(m, "cmsFLAGS_NOWHITEONWHITEFIXUP", cmsFLAGS_NOWHITEONWHITEFIXUP);
    PyModule_AddIntConstant(m, "cmsFLAGS_HIGHRESPRECALC", cmsFLAGS_HIGHRESPRECALC);
    PyModule_AddIntConstant(m, "cmsFLAGS_LOWRESPRECALC", cmsFLAGS_LOWRESPRECALC);
    PyModule_AddIntConstant(m, "cmsFLAGS_8BITS_DEVICELINK", cmsFLAGS_8BITS_DEVICELINK);
    PyModule_AddIntConstant(m, "cmsFLAGS_GUESSDEVICECLASS", cmsFLAGS_GUESSDEVICECLASS);
    PyModule_AddIntConstant(m, "cmsFLAGS_KEEP_SEQUENCE", cmsFLAGS_KEEP_SEQUENCE);
    PyModule_AddIntConstant(m, "cmsFLAGS_FORCE_CLUT", cmsFLAGS_FORCE_CLUT);
    PyModule_AddIntConstant(m, "cmsFLAGS_CLUT_POST_LINEARIZATION", cmsFLAGS_CLUT_POST_LINEARIZATION);
    PyModule_AddIntConstant(m, "cmsFLAGS_CLUT_PRE_LINEARIZATION", cmsFLAGS_CLUT_PRE_LINEARIZATION);
    PyModule_AddIntConstant(m, "cmsFLAGS_NONEGATIVES", cmsFLAGS_NONEGATIVES);
    PyModule_AddIntConstant(m, "cmsFLAGS_NODEFAULTRESOURCEDEF", cmsFLAGS_NODEFAULTRESOURCEDEF);
    PyModule_AddIntConstant(m, "cmsFLAGS_COPY_ALPHA" , cmsFLAGS_COPY_ALPHA);

    return m;

}


static PyObject *pylcms2_single_RGB_DBL_to_CMYK_DBL(PyObject *self, PyObject *args)
{
    char *outProfileName;
    cmsFloat64Number rgb[3];
    cmsFloat64Number cmyk[4];
    cmsUInt32Number intent;
    cmsUInt32Number dwFlags;

    if (! PyArg_ParseTuple(args, "dddsII", &rgb[0], &rgb[1], &rgb[2], &outProfileName, &intent, &dwFlags)) {
        return NULL;
    }

    cmsHPROFILE hInProfile, hOutProfile;
    cmsHTRANSFORM hTransform;

    hInProfile = cmsCreate_sRGBProfile();
    hOutProfile = cmsOpenProfileFromFile(outProfileName, "r");

    hTransform = cmsCreateTransform(
        hInProfile, TYPE_RGB_DBL,
        hOutProfile, TYPE_CMYK_DBL,
        intent, dwFlags);

    cmsCloseProfile(hInProfile);
    cmsCloseProfile(hOutProfile);

    cmsDoTransform(hTransform, &rgb, &cmyk, 1);
    cmsDeleteTransform(hTransform);

    PyObject *ret = Py_BuildValue("dddd", cmyk[0], cmyk[1], cmyk[2], cmyk[3]);
    return ret;
}


static PyObject *pylcms2_single_CMYK_DBL_to_Lab_DBL(PyObject *self, PyObject *args)
{
    char *outProfileName;
    cmsFloat64Number cmyk[4];
    cmsFloat64Number Lab[3];
    cmsUInt32Number intent;
    cmsUInt32Number dwFlags;

    if (! PyArg_ParseTuple(args, "ddddsII", &cmyk[0], &cmyk[1], &cmyk[2], &outProfileName, &intent, &dwFlags)) {
        return NULL;
    }

    cmsHPROFILE hInProfile, hOutProfile;
    cmsHTRANSFORM hTransform;

    hInProfile = cmsOpenProfileFromFile(outProfileName, "r");
    hOutProfile = cmsCreate_LabProfile();

    hTransform = cmsCreateTransform(
        hInProfile, TYPE_RGB_DBL,
        hOutProfile, TYPE_CMYK_DBL,
        intent, dwFlags);

    cmsCloseProfile(hInProfile);
    cmsCloseProfile(hOutProfile);

    cmsDoTransform(hTransform, &rgb, &cmyk, 1);
    cmsDeleteTransform(hTransform);

    PyObject *ret = Py_BuildValue("dddd", cmyk[0], cmyk[1], cmyk[2], cmyk[3]);
    return ret;
}