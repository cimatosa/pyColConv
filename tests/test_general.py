import pyColConv

import numpy as np


def test_get_TYPE_makros():
    """
    * checks if the member TYPE_RGB_8 (inherited from lcms) is defined
    * checks its value (which encodes information about that color format)
    """
    assert pyColConv.TYPE_RGB_8 == 262169


def test_get_channels_bytes_from_type():
    """
    test functionality of the 'get_color_space_specs' function
    """
    try:
        pyColConv.get_color_space_specs("TYPE_unknown")
    except ValueError:
        pass
    else:
        raise RuntimeError("should be treated as ValueError")

    # c: number of channels, b: number of bytes per channel, f: is float
    c, b, f = pyColConv.get_color_space_specs("TYPE_RGB_8")
    assert (c, b, f) == (3, 1, False)

    c, b, f = pyColConv.get_color_space_specs("TYPE_ARGB_16")
    assert (c, b, f) == (4, 2, False)

    c, b, f = pyColConv.get_color_space_specs("TYPE_CMYK_FLT")
    assert (c, b, f) == (4, 4, True)

    c, b, f = pyColConv.get_color_space_specs("TYPE_Lab_DBL")
    assert (c, b, f) == (3, 8, True)


def test_do_transform():
    """
    test functionality of the conversion

        1. identity transform SRGB -> SRGB
        2. RGB white [1.0, 1.0, 1.0] -> Lab ~ [100.0, 0.0, 0.0]
    """

    # identity transform SRGB -> SRGB
    pix = 2
    in_data = np.asarray(np.arange(3 * pix), dtype=np.uint8)
    out_data = pyColConv.convert(
        in_prof=pyColConv.PROFILE_SRGB,
        in_Py_buffer=in_data.data,
        in_fmt="TYPE_RGB_8",
        out_prof=pyColConv.PROFILE_SRGB,
        out_fmt="TYPE_RGB_8",
        intent=pyColConv.INTENT_ABSOLUTE_COLORIMETRIC,
        dw_flags=0,
    )
    for i in range(pix * 3):
        assert in_data[i] == out_data[i]

    # RGB white [1.0, 1.0, 1.0] -> Lab ~ [100.0, 0.0, 0.0]
    in_data = np.asarray([1, 1, 1], dtype="f8")
    out_data = pyColConv.convert(
        in_prof=pyColConv.PROFILE_SRGB,
        in_Py_buffer=in_data.data,
        in_fmt="TYPE_RGB_DBL",
        out_prof=pyColConv.PROFILE_Lab_D50,
        out_fmt="TYPE_Lab_DBL",
        intent=pyColConv.INTENT_ABSOLUTE_COLORIMETRIC,
        dw_flags=0,
    )
    out_data = np.frombuffer(out_data, dtype="f8")
    assert abs(out_data[0] - 100) < 1e-5
    assert abs(out_data[1]) < 1e-5
    assert abs(out_data[2]) < 1e-5


def test_nparray():
    r=0.6
    g=0.2
    b=0.3

    c_ref = np.asarray([13.20973486, 90.42343497, 41.03150964, 36.78644896])

    for (out_fmt, out_fmt_name) in [(pyColConv.TYPE_CMYK_DBL, 'float64'),
                                    (pyColConv.TYPE_CMYK_FLT, 'float32')]:

        for t in ['f4', 'f8']:
            c_sRGB = np.asarray([r,g,b], dtype=t)
            c_CMYK = pyColConv.convert_nparray(
                in_prof=pyColConv.PROFILE_SRGB,
                in_array=c_sRGB,
                in_pixel_type=pyColConv.PT_RGB,
                out_prof="../data/PSOcoated_v3.icc",
                out_fmt=out_fmt,
                intent=pyColConv.INTENT_RELATIVE_COLORIMETRIC,
                dw_flags=0
            )
            assert np.allclose(c_ref, c_CMYK)
            assert (c_CMYK.dtype.name == out_fmt_name)

        m = 2**8
        c_sRGB = np.asarray([m*r, m*g, m*b], dtype='u1')
        c_CMYK = pyColConv.convert_nparray(
            in_prof=pyColConv.PROFILE_SRGB,
            in_array=c_sRGB,
            in_pixel_type=pyColConv.PT_RGB,
            out_prof="../data/PSOcoated_v3.icc",
            out_fmt=out_fmt,
            intent=pyColConv.INTENT_RELATIVE_COLORIMETRIC,
            dw_flags=0
        )
        d = np.max(np.abs(c_ref - c_CMYK))
        assert d < 0.7
        assert(c_CMYK.dtype.name == out_fmt_name)

        m = 2 ** 16
        c_sRGB = np.asarray([m * r, m * g, m * b], dtype='u2')
        c_CMYK = pyColConv.convert_nparray(
            in_prof=pyColConv.PROFILE_SRGB,
            in_array=c_sRGB,
            in_pixel_type=pyColConv.PT_RGB,
            out_prof="../data/PSOcoated_v3.icc",
            out_fmt=out_fmt,
            intent=pyColConv.INTENT_RELATIVE_COLORIMETRIC,
            dw_flags=0
        )
        d = np.max(np.abs(c_ref - c_CMYK))
        assert d < 0.014
        assert (c_CMYK.dtype.name == out_fmt_name)





if __name__ == "__main__":
    pyColConv.useDebugMode()
    # test_get_TYPE_makros()
    # test_get_channels_bytes_from_type()
    # test_do_transform()
    test_nparray()
