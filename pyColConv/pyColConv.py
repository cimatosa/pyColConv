import logging
import _lcms2
import numpy as np
from typing import Union, Any, Tuple
from . import util_lcms

# ------------------------
#     setup logger
# ------------------------
log = logging.getLogger(__name__)



def useDebugMode():
    """
    sets logging level and creates a stream handler to show full debugging information
    """
    log.setLevel(logging.DEBUG)
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)
    formatter = logging.Formatter("%(name)s - %(levelname)s - %(message)s")
    ch.setFormatter(formatter)
    log.addHandler(ch)

def get_color_space_number(fmt: Union[str, int]) -> int:
    """
    Get color space number by name.
    If the number is already given, simply pass it on

    :param fmt: name of the colorspace in lcms2 specific format as string, or the corresponding number
    :return: the number for that color space
    """
    if isinstance(fmt, str):
        try:
            colorspace_number = getattr(_lcms2, fmt)
        except AttributeError:
            raise ValueError(
                "color space format '{}' not found!\n".format(fmt)
                + "If it is defined in lcms2.h it should be added to 'PyInit__lcms2' in '_lcms2.c"
            )
    else:
        colorspace_number = fmt

    return colorspace_number


def get_color_space_specs(cs_number: int) -> Tuple[int, int, int]:
    """
    Infer binary layout of the colorspace number 'cs_number' (see lcms2.h).
    Use 'get_color_space_number' to get the number from the string name of the color space.
    The numbers are also defined as members starting with 'TYPE_'.

    That includes
        * the total number of channels (channels + extra channels)
        * bytes per channel (where 0 means 8 as used for doubles)
        * floating point or integer

    :param cs_number: the color space number cs_number
    :return: tuple (total number of channels, bytes per channel, is floating point)
    """

    B = util_lcms.T_BYTES(cs_number)
    C = util_lcms.T_CHANNELS(cs_number)
    E = util_lcms.T_EXTRA(cs_number)
    A = util_lcms.T_FLOAT(cs_number)

    if B == 0:
        B = 8
    log.debug(
        "colorspace fmt '{}', channels={}, bytes={}, isFloat={}".format(
            cs_number, C + E, B, bool(A)
        )
    )
    return C + E, B, bool(A)


def convert(
    in_prof: str,
    in_Py_buffer: Any,  # anything that supports the buffer interface
    in_fmt: Union[str, int],
    out_prof: str,
    out_fmt: Union[str, int],
    intent: int,
    dw_flags: int,
):
    """
    convert between color spaces

    This variant is closest to the underlying 'cmsDoTransform' function in the sense that
    the inpout data 'in_Py_buffer' is provided by a generic python buffer (raw binary data)
    while its format is given separately by 'in_fmt'. The result is returned as python buffer
    as well with a binary format according to 'out_fmt'.

    The possible formats corresponds to the predefined values from lcms2.h. Their numeric value
    encodes the properties how to read the binary data. As example
        - TYPE_RGB_8: 3 channels R,G,B with 8 bit (one byte) per channel
        - TYPE_CMYK_DBL: 4 channels C,M,Y,K of doubles (8 byte) per channel

    Together with the input profile 'in_prof', output profile 'out_prof', the rendering intent 'intent'
    and further flags 'dw_flags' the color transformation is fully specified.

    The profiles need to be given as string, either meaning a filename or referring to the predefined
    profiles:
        - PROFILE_SRGB = '*sRGB'
        - PROFILE_Lab_D50 = '*Lab_D50'

    The possible intent value are encoded in
        - INTENT_PERCEPTUAL = _lcms2.INTENT_PERCEPTUAL
        - INTENT_RELATIVE_COLORIMETRIC = _lcms2.INTENT_RELATIVE_COLORIMETRIC
        - INTENT_SATURATION = _lcms2.INTENT_SATURATION
        - INTENT_ABSOLUTE_COLORIMETRIC = _lcms2.INTENT_ABSOLUTE_COLORIMETRIC
    and match the values of the lcms2 implementation.

    The same holds for the dw_flags (see little cms docs). As an example we name
        - cmsFLAGS_BLACKPOINTCOMPENSATION
    which enables blackpoint compensation.

    :param in_prof     : the input profile as string (filename of predefined values starting with *)
    :param in_Py_buffer: the input data as raw binary buffer
    :param in_fmt      : the format of the input data (see pyColConv members starting with TYPE_)
    :param out_prof    : the output profile as string (filename of predefined values starting with *)
    :param out_fmt     : the format of the output data (see pyColConv members starting with TYPE_)
    :param intent      : the rendering intent (INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC,
                         INTENT_SATURATION, INTENT_ABSOLUTE_COLORIMETRIC)
    :param dw_flags    : additional flags for the conversion, like cmsFLAGS_BLACKPOINTCOMPENSATION
    :return            : bytearray (Py_buffer) containing raw binary data of the output colors
    """
    in_C, in_B, in_F = get_color_space_specs(in_fmt)
    out_C, out_B, out_F = get_color_space_specs(out_fmt)

    num_pixels = len(in_Py_buffer) // in_C
    log.debug(
        "{} pixels determined from input buffer on length {}".format(
            num_pixels, len(in_Py_buffer)
        )
    )

    out_Py_buffer = bytearray(num_pixels * out_C * out_B)
    log.debug("create output buffer with {} bytes".format(len(out_Py_buffer)))

    ret_code, error_msg = _lcms2.do_transform(
        in_prof,
        out_prof,
        in_Py_buffer,
        in_fmt,
        out_Py_buffer,
        out_fmt,
        intent,
        dw_flags,
        num_pixels,
    )
    log.debug("calling 'do_transform' from lcms2 returned '{}'".format(ret_code))
    if ret_code != 0:
        raise RuntimeError(
            "calling 'do_transform' from lcms2 failed with error '{}'".format(error_msg)
        )

    return out_Py_buffer


def convert_nparray(
    in_prof: str,
    in_array: np.ndarray,
    in_pixel_type: int,
    out_prof: str,
    out_fmt: Union[str, int],
    intent: int,
    dw_flags: int,
):
    """
    convert between color spaces

    This variant does the same job as 'convert' with the difference that it expects
    the input data and returns the output data as numpy array.

    For the input data the pixel type need to be specified (see lcms2.h). They as provided
    as module members starting with PT_. The number representation is deduced from the
    type of the input array.

    The format of the output is set by 'out_fmt'.

    The possible formats corresponds to the predefined values from lcms2.h. Their numeric value
    encodes the properties how to read the binary data. As example
        - TYPE_RGB_8: 3 channels R,G,B with 8 bit (one byte) per channel
        - TYPE_CMYK_DBL: 4 channels C,M,Y,K of doubles (8 byte) per channel

    Together with the input profile 'in_prof', output profile 'out_prof', the rendering intent 'intent'
    and further flags 'dw_flags' the color transformation is fully specified.

    The profiles need to be given as string, either meaning a filename or referring to the predefined
    profiles:
        - PROFILE_SRGB = '*sRGB'
        - PROFILE_Lab_D50 = '*Lab_D50'

    The possible intent value are encoded in
        - INTENT_PERCEPTUAL = _lcms2.INTENT_PERCEPTUAL
        - INTENT_RELATIVE_COLORIMETRIC = _lcms2.INTENT_RELATIVE_COLORIMETRIC
        - INTENT_SATURATION = _lcms2.INTENT_SATURATION
        - INTENT_ABSOLUTE_COLORIMETRIC = _lcms2.INTENT_ABSOLUTE_COLORIMETRIC
    and match the values of the lcms2 implementation.

    The same holds for the dw_flags (see little cms docs). As an example we name
        - cmsFLAGS_BLACKPOINTCOMPENSATION
    which enables blackpoint compensation.

    :param in_prof       : the input profile as string (filename of predefined values starting with *)
    :param in_array      : the input data as numpy array.
    :param in_pixel_type : the pixel type (see pyColConv members starting with PT_)
    :param out_prof      : the output profile as string (filename of predefined values starting with *)
    :param out_fmt       : the format of the output data (see pyColConv members starting with TYPE_)
    :param intent        : the rendering intent (INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC,
                           INTENT_SATURATION, INTENT_ABSOLUTE_COLORIMETRIC)
    :param dw_flags      : additional flags for the conversion, like cmsFLAGS_BLACKPOINTCOMPENSATION
    :return              : numpy array with output colors
    """

    in_C = util_lcms.number_of_channels(in_pixel_type)
    log.debug("'in_pixel_type' has {} channels".format(in_C))

    dt = in_array.dtype
    #if (dt.byteorder != '='):
    #    raise ValueError("Numpy array has has invalid type! "+
    #                     "Byteorder is '{}'. Byteorder must be native.".format(dt.byteorder))
    if dt.kind == 'u':      # unsigned int
        in_F = 0
        in_B = dt.alignment
        if (in_B != 1) and (in_B != 2):
            raise ValueError("Numpy array has has invalid type! "+
                             "dtype.alignment yields {} byte(s). Only uint8 and uint16 are supported.".format(in_B))
    elif dt.kind == 'f':    # floating point
        in_F = 1
        in_B = dt.alignment
        if (in_B != 4) and (in_B != 8):
            raise ValueError("Numpy array has has invalid type! "+
                             "Only float (32 bit) and double (64 bit) are supported.")
        if in_B == 8:
            in_B = 0   # due to overflow in 3-bit representation use 0
    else:
        raise ValueError("numpy data type '{}' is not supported".format(dt))
    log.debug("'in_array' has dtype {} which maps to num_bytes:{} and is_float:{}".format(dt, in_B, in_F))

    in_fmt = ( util_lcms.FLOAT_SH(in_F) |
               util_lcms.COLORSPACE_SH(in_pixel_type) |
               util_lcms.CHANNELS_SH(in_C) |
               util_lcms.BYTES_SH(in_B) )
    log.debug("the corresponding 'in_fmt' is {}".format(in_fmt))

    out_data = convert(
        in_prof = in_prof,
        in_Py_buffer=in_array.data,
        in_fmt=in_fmt,
        out_prof=out_prof,
        out_fmt=out_fmt,
        intent=intent,
        dw_flags=dw_flags
    )
    log.debug("convert successful!")

    out_C, out_B, out_F = get_color_space_specs(out_fmt)
    if out_F:
        out_dt = 'f'        # float
    else:
        out_dt = 'u'        # uint
    out_dt += str(out_B)
    log.debug("numpy dtype for output from 'out_fmt' is {}".format(out_dt))
    out_array = np.frombuffer(out_data, dtype=out_dt)
    return out_array



