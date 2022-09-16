import pylcms2

import numpy as np

def test_pass_nparay_data():
    """
    simple test to pass nparay to c-code and manipulate its data

        - a.data is Python buffer object pointing to the data of a
        - in c PyArg_ParseTuple(args, "y*", &in_buf) receives that as Py_buffer
        - the .buf member of that struct holds the address
    """
    a = np.zeros(5, dtype=np.uint32)
    print(a)

    itemsize_from_c = pylcms2.dev_get_nparray_as_Py_buffer(a.data)

    print(a)
    print(itemsize_from_c)

    assert a[0] == 4
    assert a[1] == 0
    assert a[2] == 3

    assert itemsize_from_c == 5

def test_get_TYPE_makros():
    assert pylcms2.TYPE_RGB_8 == 262169

def test_get_channels_bytes_from_type():
    try:
        pylcms2.get_color_space_specs('TYPE_unknown')
    except ValueError:
        pass
    else:
        raise RuntimeError("should be treated as ValueError")

    c, b, f = pylcms2.get_color_space_specs('TYPE_RGB_8')
    assert (c,b,f) == (3,1,False)

    c, b, f = pylcms2.get_color_space_specs('TYPE_ARGB_16')
    assert (c, b, f) == (4, 2, False)

    c, b, f = pylcms2.get_color_space_specs('TYPE_CMYK_FLT')
    assert (c, b, f) == (4, 4, True)

    c, b, f = pylcms2.get_color_space_specs('TYPE_Lab_DBL')
    assert (c, b, f) == (3, 8, True)


def test_do_transform():
    pix = 2
    in_data = np.asarray(np.arange(3*pix), dtype=np.uint8)
    out_data = pylcms2.do_transform(
        in_prof=pylcms2.PROFILE_SRGB,
        in_Py_buffer=in_data.data,
        in_fmt='TYPE_RGB_8',
        out_prof=pylcms2.PROFILE_SRGB,
        out_fmt='TYPE_RGB_8',
        intent=pylcms2.INTENT_ABSOLUTE_COLORIMETRIC,
        dw_flags=0
    )
    for i in range(pix*3):
        assert in_data[i] == out_data[i]

    in_data = np.asarray([1, 1, 1], dtype='f8')
    out_data = pylcms2.do_transform(
        in_prof=pylcms2.PROFILE_SRGB,
        in_Py_buffer=in_data.data,
        in_fmt='TYPE_RGB_DBL',
        out_prof=pylcms2.PROFILE_Lab_D50,
        out_fmt='TYPE_Lab_DBL',
        intent=pylcms2.INTENT_ABSOLUTE_COLORIMETRIC,
        dw_flags=0
    )
    out_data = np.frombuffer(out_data, dtype='f8')
    assert abs(out_data[0] - 100) < 1e-5
    assert abs(out_data[1]) < 1e-5
    assert abs(out_data[2]) < 1e-5

if __name__ == "__main__":
    pylcms2.useDebugMode()
    # test_pass_nparay_data()
    # test_get_TYPE_makros()
    # test_get_channels_bytes_from_type()
    test_do_transform()
