#include "lcms2.h"
#include "string.h"
#include <Python.h>

static char module_docstring[] = "provides color transformation dones by lcms2 as pyton function";
static char do_transform_docstring[] = "loads/creates the input/output profile and apply the transformation to the input ata";
static char dev_get_nparray_as_Py_buffer_doc[] = "get access to a nparray (dev function)";

// declaration of functions
static PyObject *pylcms2_do_transform(PyObject *self, PyObject *args);
static PyObject *pylcms2_dev_get_nparray_as_Py_buffer(PyObject *self, PyObject *args);

// declaration of python-module methods
static PyMethodDef module_methods[] = {
    {"do_transform", pylcms2_do_transform, METH_VARARGS, do_transform_docstring},
    {"dev_get_nparray_as_Py_buffer", pylcms2_dev_get_nparray_as_Py_buffer, METH_VARARGS, dev_get_nparray_as_Py_buffer_doc},
    {NULL, NULL, 0, NULL}
};

// declaration of python-module
static struct PyModuleDef lcms2_module = {
    PyModuleDef_HEAD_INIT,
    "_lcms2",              /* name of module */
    module_docstring,     /* module documentation, may be NULL */
    -1,                   /* size of per-interpreter state of the module,
                             or -1 if the module keeps state in global variables. */
    module_methods
};

// init python module
PyMODINIT_FUNC PyInit__lcms2(void)
{
    /* Load `numpy` functionality. */
//    import_array();

    PyObject * m = PyModule_Create(&lcms2_module);

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
//  define color spaces
    PyModule_AddIntConstant(m, "TYPE_GRAY_8", TYPE_GRAY_8);
    PyModule_AddIntConstant(m, "TYPE_GRAY_8_REV", TYPE_GRAY_8_REV);
    PyModule_AddIntConstant(m, "TYPE_GRAY_16", TYPE_GRAY_16);
    PyModule_AddIntConstant(m, "TYPE_GRAY_16_REV", TYPE_GRAY_16_REV);
    PyModule_AddIntConstant(m, "TYPE_GRAY_16_SE", TYPE_GRAY_16_SE);
    PyModule_AddIntConstant(m, "TYPE_GRAYA_8", TYPE_GRAYA_8);
    PyModule_AddIntConstant(m, "TYPE_GRAYA_16", TYPE_GRAYA_16);
    PyModule_AddIntConstant(m, "TYPE_GRAYA_16_SE", TYPE_GRAYA_16_SE);
    PyModule_AddIntConstant(m, "TYPE_GRAYA_8_PLANAR", TYPE_GRAYA_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_GRAYA_16_PLANAR", TYPE_GRAYA_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_RGB_8", TYPE_RGB_8);
    PyModule_AddIntConstant(m, "TYPE_RGB_8_PLANAR", TYPE_RGB_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_BGR_8", TYPE_BGR_8);
    PyModule_AddIntConstant(m, "TYPE_BGR_8_PLANAR", TYPE_BGR_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_RGB_16", TYPE_RGB_16);
    PyModule_AddIntConstant(m, "TYPE_RGB_16_PLANAR", TYPE_RGB_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_RGB_16_SE", TYPE_RGB_16_SE);
    PyModule_AddIntConstant(m, "TYPE_BGR_16", TYPE_BGR_16);
    PyModule_AddIntConstant(m, "TYPE_BGR_16_PLANAR", TYPE_BGR_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_BGR_16_SE", TYPE_BGR_16_SE);
    PyModule_AddIntConstant(m, "TYPE_RGBA_8", TYPE_RGBA_8);
    PyModule_AddIntConstant(m, "TYPE_RGBA_8_PLANAR", TYPE_RGBA_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_RGBA_16", TYPE_RGBA_16);
    PyModule_AddIntConstant(m, "TYPE_RGBA_16_PLANAR", TYPE_RGBA_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_RGBA_16_SE", TYPE_RGBA_16_SE);
    PyModule_AddIntConstant(m, "TYPE_ARGB_8", TYPE_ARGB_8);
    PyModule_AddIntConstant(m, "TYPE_ARGB_8_PLANAR", TYPE_ARGB_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_ARGB_16", TYPE_ARGB_16);
    PyModule_AddIntConstant(m, "TYPE_ABGR_8", TYPE_ABGR_8);
    PyModule_AddIntConstant(m, "TYPE_ABGR_8_PLANAR", TYPE_ABGR_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_ABGR_16", TYPE_ABGR_16);
    PyModule_AddIntConstant(m, "TYPE_ABGR_16_PLANAR", TYPE_ABGR_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_ABGR_16_SE", TYPE_ABGR_16_SE);
    PyModule_AddIntConstant(m, "TYPE_BGRA_8", TYPE_BGRA_8);
    PyModule_AddIntConstant(m, "TYPE_BGRA_8_PLANAR", TYPE_BGRA_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_BGRA_16", TYPE_BGRA_16);
    PyModule_AddIntConstant(m, "TYPE_BGRA_16_SE", TYPE_BGRA_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMY_8", TYPE_CMY_8);
    PyModule_AddIntConstant(m, "TYPE_CMY_8_PLANAR", TYPE_CMY_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_CMY_16", TYPE_CMY_16);
    PyModule_AddIntConstant(m, "TYPE_CMY_16_PLANAR", TYPE_CMY_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_CMY_16_SE", TYPE_CMY_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK_8", TYPE_CMYK_8);
    PyModule_AddIntConstant(m, "TYPE_CMYKA_8", TYPE_CMYKA_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK_8_REV", TYPE_CMYK_8_REV);
    PyModule_AddIntConstant(m, "TYPE_YUVK_8", TYPE_YUVK_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK_8_PLANAR", TYPE_CMYK_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_CMYK_16", TYPE_CMYK_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK_16_REV", TYPE_CMYK_16_REV);
    PyModule_AddIntConstant(m, "TYPE_YUVK_16", TYPE_YUVK_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK_16_PLANAR", TYPE_CMYK_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_CMYK_16_SE", TYPE_CMYK_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KYMC_8", TYPE_KYMC_8);
    PyModule_AddIntConstant(m, "TYPE_KYMC_16", TYPE_KYMC_16);
    PyModule_AddIntConstant(m, "TYPE_KYMC_16_SE", TYPE_KYMC_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KCMY_8", TYPE_KCMY_8);
    PyModule_AddIntConstant(m, "TYPE_KCMY_8_REV", TYPE_KCMY_8_REV);
    PyModule_AddIntConstant(m, "TYPE_KCMY_16", TYPE_KCMY_16);
    PyModule_AddIntConstant(m, "TYPE_KCMY_16_REV", TYPE_KCMY_16_REV);
    PyModule_AddIntConstant(m, "TYPE_KCMY_16_SE", TYPE_KCMY_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK5_8", TYPE_CMYK5_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK5_16", TYPE_CMYK5_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK5_16_SE", TYPE_CMYK5_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KYMC5_8", TYPE_KYMC5_8);
    PyModule_AddIntConstant(m, "TYPE_KYMC5_16", TYPE_KYMC5_16);
    PyModule_AddIntConstant(m, "TYPE_KYMC5_16_SE", TYPE_KYMC5_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK6_8", TYPE_CMYK6_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK6_8_PLANAR", TYPE_CMYK6_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_CMYK6_16", TYPE_CMYK6_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK6_16_PLANAR", TYPE_CMYK6_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_CMYK6_16_SE", TYPE_CMYK6_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK7_8", TYPE_CMYK7_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK7_16", TYPE_CMYK7_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK7_16_SE", TYPE_CMYK7_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KYMC7_8", TYPE_KYMC7_8);
    PyModule_AddIntConstant(m, "TYPE_KYMC7_16", TYPE_KYMC7_16);
    PyModule_AddIntConstant(m, "TYPE_KYMC7_16_SE", TYPE_KYMC7_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK8_8", TYPE_CMYK8_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK8_16", TYPE_CMYK8_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK8_16_SE", TYPE_CMYK8_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KYMC8_8", TYPE_KYMC8_8);
    PyModule_AddIntConstant(m, "TYPE_KYMC8_16", TYPE_KYMC8_16);
    PyModule_AddIntConstant(m, "TYPE_KYMC8_16_SE", TYPE_KYMC8_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK9_8", TYPE_CMYK9_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK9_16", TYPE_CMYK9_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK9_16_SE", TYPE_CMYK9_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KYMC9_8", TYPE_KYMC9_8);
    PyModule_AddIntConstant(m, "TYPE_KYMC9_16", TYPE_KYMC9_16);
    PyModule_AddIntConstant(m, "TYPE_KYMC9_16_SE", TYPE_KYMC9_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK10_8", TYPE_CMYK10_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK10_16", TYPE_CMYK10_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK10_16_SE", TYPE_CMYK10_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KYMC10_8", TYPE_KYMC10_8);
    PyModule_AddIntConstant(m, "TYPE_KYMC10_16", TYPE_KYMC10_16);
    PyModule_AddIntConstant(m, "TYPE_KYMC10_16_SE", TYPE_KYMC10_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK11_8", TYPE_CMYK11_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK11_16", TYPE_CMYK11_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK11_16_SE", TYPE_CMYK11_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KYMC11_8", TYPE_KYMC11_8);
    PyModule_AddIntConstant(m, "TYPE_KYMC11_16", TYPE_KYMC11_16);
    PyModule_AddIntConstant(m, "TYPE_KYMC11_16_SE", TYPE_KYMC11_16_SE);
    PyModule_AddIntConstant(m, "TYPE_CMYK12_8", TYPE_CMYK12_8);
    PyModule_AddIntConstant(m, "TYPE_CMYK12_16", TYPE_CMYK12_16);
    PyModule_AddIntConstant(m, "TYPE_CMYK12_16_SE", TYPE_CMYK12_16_SE);
    PyModule_AddIntConstant(m, "TYPE_KYMC12_8", TYPE_KYMC12_8);
    PyModule_AddIntConstant(m, "TYPE_KYMC12_16", TYPE_KYMC12_16);
    PyModule_AddIntConstant(m, "TYPE_KYMC12_16_SE", TYPE_KYMC12_16_SE);
    PyModule_AddIntConstant(m, "TYPE_XYZ_16", TYPE_XYZ_16);
    PyModule_AddIntConstant(m, "TYPE_Lab_8", TYPE_Lab_8);
    PyModule_AddIntConstant(m, "TYPE_LabV2_8", TYPE_LabV2_8);
    PyModule_AddIntConstant(m, "TYPE_ALab_8", TYPE_ALab_8);
    PyModule_AddIntConstant(m, "TYPE_ALabV2_8", TYPE_ALabV2_8);
    PyModule_AddIntConstant(m, "TYPE_Lab_16", TYPE_Lab_16);
    PyModule_AddIntConstant(m, "TYPE_LabV2_16", TYPE_LabV2_16);
    PyModule_AddIntConstant(m, "TYPE_Yxy_16", TYPE_Yxy_16);
    PyModule_AddIntConstant(m, "TYPE_YCbCr_8", TYPE_YCbCr_8);
    PyModule_AddIntConstant(m, "TYPE_YCbCr_8_PLANAR", TYPE_YCbCr_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_YCbCr_16", TYPE_YCbCr_16);
    PyModule_AddIntConstant(m, "TYPE_YCbCr_16_PLANAR", TYPE_YCbCr_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_YCbCr_16_SE", TYPE_YCbCr_16_SE);
    PyModule_AddIntConstant(m, "TYPE_YUV_8", TYPE_YUV_8);
    PyModule_AddIntConstant(m, "TYPE_YUV_8_PLANAR", TYPE_YUV_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_YUV_16", TYPE_YUV_16);
    PyModule_AddIntConstant(m, "TYPE_YUV_16_PLANAR", TYPE_YUV_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_YUV_16_SE", TYPE_YUV_16_SE);
    PyModule_AddIntConstant(m, "TYPE_HLS_8", TYPE_HLS_8);
    PyModule_AddIntConstant(m, "TYPE_HLS_8_PLANAR", TYPE_HLS_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_HLS_16", TYPE_HLS_16);
    PyModule_AddIntConstant(m, "TYPE_HLS_16_PLANAR", TYPE_HLS_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_HLS_16_SE", TYPE_HLS_16_SE);
    PyModule_AddIntConstant(m, "TYPE_HSV_8", TYPE_HSV_8);
    PyModule_AddIntConstant(m, "TYPE_HSV_8_PLANAR", TYPE_HSV_8_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_HSV_16", TYPE_HSV_16);
    PyModule_AddIntConstant(m, "TYPE_HSV_16_PLANAR", TYPE_HSV_16_PLANAR);
    PyModule_AddIntConstant(m, "TYPE_HSV_16_SE", TYPE_HSV_16_SE);
    PyModule_AddIntConstant(m, "TYPE_NAMED_COLOR_INDEX", TYPE_NAMED_COLOR_INDEX);
    PyModule_AddIntConstant(m, "TYPE_XYZ_FLT", TYPE_XYZ_FLT);
    PyModule_AddIntConstant(m, "TYPE_Lab_FLT", TYPE_Lab_FLT);
    PyModule_AddIntConstant(m, "TYPE_LabA_FLT", TYPE_LabA_FLT);
    PyModule_AddIntConstant(m, "TYPE_GRAY_FLT", TYPE_GRAY_FLT);
    PyModule_AddIntConstant(m, "TYPE_RGB_FLT", TYPE_RGB_FLT);
    PyModule_AddIntConstant(m, "TYPE_RGBA_FLT", TYPE_RGBA_FLT);
    PyModule_AddIntConstant(m, "TYPE_ARGB_FLT", TYPE_ARGB_FLT);
    PyModule_AddIntConstant(m, "TYPE_BGR_FLT", TYPE_BGR_FLT);
    PyModule_AddIntConstant(m, "TYPE_BGRA_FLT", TYPE_BGRA_FLT);
    PyModule_AddIntConstant(m, "TYPE_ABGR_FLT", TYPE_ABGR_FLT);
    PyModule_AddIntConstant(m, "TYPE_CMYK_FLT", TYPE_CMYK_FLT);
    PyModule_AddIntConstant(m, "TYPE_XYZ_DBL", TYPE_XYZ_DBL);
    PyModule_AddIntConstant(m, "TYPE_Lab_DBL", TYPE_Lab_DBL);
    PyModule_AddIntConstant(m, "TYPE_GRAY_DBL", TYPE_GRAY_DBL);
    PyModule_AddIntConstant(m, "TYPE_RGB_DBL", TYPE_RGB_DBL);
    PyModule_AddIntConstant(m, "TYPE_BGR_DBL", TYPE_BGR_DBL);
    PyModule_AddIntConstant(m, "TYPE_CMYK_DBL", TYPE_CMYK_DBL);
    PyModule_AddIntConstant(m, "TYPE_GRAY_HALF_FLT", TYPE_GRAY_HALF_FLT);
    PyModule_AddIntConstant(m, "TYPE_RGB_HALF_FLT", TYPE_RGB_HALF_FLT);
    PyModule_AddIntConstant(m, "TYPE_RGBA_HALF_FLT", TYPE_RGBA_HALF_FLT);
    PyModule_AddIntConstant(m, "TYPE_CMYK_HALF_FLT", TYPE_CMYK_HALF_FLT);
    PyModule_AddIntConstant(m, "TYPE_RGBA_HALF_FLT", TYPE_RGBA_HALF_FLT);
    PyModule_AddIntConstant(m, "TYPE_ARGB_HALF_FLT", TYPE_ARGB_HALF_FLT);
    PyModule_AddIntConstant(m, "TYPE_BGR_HALF_FLT", TYPE_BGR_HALF_FLT);
    PyModule_AddIntConstant(m, "TYPE_BGRA_HALF_FLT", TYPE_BGRA_HALF_FLT);
    PyModule_AddIntConstant(m, "TYPE_ABGR_HALF_FLT", TYPE_ABGR_HALF_FLT);

    PyModule_AddIntConstant(m, "PT_GRAY", PT_GRAY);
    PyModule_AddIntConstant(m, "PT_RGB", PT_RGB);
    PyModule_AddIntConstant(m, "PT_CMY", PT_CMY);
    PyModule_AddIntConstant(m, "PT_CMYK", PT_CMYK);
    PyModule_AddIntConstant(m, "PT_YCbCr", PT_YCbCr);
    PyModule_AddIntConstant(m, "PT_YUV", PT_YUV);
    PyModule_AddIntConstant(m, "PT_XYZ", PT_XYZ);
    PyModule_AddIntConstant(m, "PT_Lab", PT_Lab);
    PyModule_AddIntConstant(m, "PT_YUVK", PT_YUVK);
    PyModule_AddIntConstant(m, "PT_HSV", PT_HSV);
    PyModule_AddIntConstant(m, "PT_HLS", PT_HLS);
    PyModule_AddIntConstant(m, "PT_Yxy", PT_Yxy);
    PyModule_AddIntConstant(m, "PT_MCH1", PT_MCH1);
    PyModule_AddIntConstant(m, "PT_MCH2", PT_MCH2);
    PyModule_AddIntConstant(m, "PT_MCH3", PT_MCH3);
    PyModule_AddIntConstant(m, "PT_MCH4", PT_MCH4);
    PyModule_AddIntConstant(m, "PT_MCH5", PT_MCH5);
    PyModule_AddIntConstant(m, "PT_MCH6", PT_MCH6);
    PyModule_AddIntConstant(m, "PT_MCH7", PT_MCH7);
    PyModule_AddIntConstant(m, "PT_MCH8", PT_MCH8);
    PyModule_AddIntConstant(m, "PT_MCH9", PT_MCH9);
    PyModule_AddIntConstant(m, "PT_MCH10", PT_MCH10);
    PyModule_AddIntConstant(m, "PT_MCH11", PT_MCH11);
    PyModule_AddIntConstant(m, "PT_MCH12", PT_MCH12);
    PyModule_AddIntConstant(m, "PT_MCH13", PT_MCH13);
    PyModule_AddIntConstant(m, "PT_MCH14", PT_MCH14);
    PyModule_AddIntConstant(m, "PT_MCH15", PT_MCH15);
    PyModule_AddIntConstant(m, "PT_LabV2", PT_LabV2);

    return m;
}


static cmsHPROFILE createProfile(char *profileName)
{
    cmsHPROFILE hProfile;
    if (strcmp(profileName, "*sRGB") == 0)
    {
        hProfile = cmsCreate_sRGBProfile();
    } else if (strcmp(profileName, "*Lab_D50") == 0)
    {
        hProfile = cmsCreateLab4Profile(NULL);
    } else
    {
        hProfile = cmsOpenProfileFromFile(profileName, "r");
    }
    return hProfile;
}

static PyObject *pylcms2_dev_get_nparray_as_Py_buffer(PyObject *self, PyObject *args)
{

    Py_buffer in_buf;
    int ok;

    ok = PyArg_ParseTuple(args, "y*", &in_buf);

    if (!ok)
    {
        return NULL;
    }

    char * in_data_ptr = in_buf.buf;
    uint32_t* intdata =(uint32_t*) in_data_ptr;

    intdata[0] = 4;
    intdata[2] = 3;
    PyObject *ret = Py_BuildValue("I", in_buf.len / in_buf.itemsize);
    return ret;

}


static PyObject *pylcms2_do_transform(PyObject *self, PyObject *args)
{
    char *inProfileName;
    char *outProfileName;
    Py_buffer inData;
    cmsUInt32Number inDataFormat;
    Py_buffer outData;
    cmsUInt32Number outDataFormat;
    cmsUInt32Number intent;
    cmsUInt32Number dwFlags;
    cmsUInt32Number dataSize;

    int ok;

    ok = PyArg_ParseTuple(
        args,
        "ssy*Iy*IIII",
        &inProfileName,
        &outProfileName,
        &inData,
        &inDataFormat,
        &outData,
        &outDataFormat,
        &intent,
        &dwFlags,
        &dataSize
        );

    if (!ok)
    {
        return NULL;
    }

    cmsHPROFILE hInProfile, hOutProfile;
    cmsHTRANSFORM hTransform;

    hInProfile = createProfile(inProfileName);
    if (hInProfile == NULL)
    {
        PyBuffer_Release(&inData);
        PyBuffer_Release(&outData);
        PyObject *ret = Py_BuildValue("is", -1, "failed to create the input profile");
        return ret;
    }
    hOutProfile = createProfile(outProfileName);
    if (hOutProfile == NULL)
    {
        PyBuffer_Release(&inData);
        PyBuffer_Release(&outData);
        PyObject *ret = Py_BuildValue("is", -1, "failed to create the output profile");
        return ret;
    }

    hTransform = cmsCreateTransform(
        hInProfile, inDataFormat,
        hOutProfile, outDataFormat,
        intent, dwFlags);
    if (hTransform == NULL)
    {
        PyBuffer_Release(&inData);
        PyBuffer_Release(&outData);
        PyObject *ret = Py_BuildValue("is", -1, "failed to create the transform");
        return ret;
    }

    cmsCloseProfile(hInProfile);
    cmsCloseProfile(hOutProfile);

    cmsDoTransform(hTransform, inData.buf, outData.buf, dataSize);
    cmsDeleteTransform(hTransform);

    PyBuffer_Release(&inData);
    PyBuffer_Release(&outData);

    PyObject *ret = Py_BuildValue("is", 0, "success");
    return ret;
}