import logging
import _lcms2
import numpy as np

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
    formatter = logging.Formatter('%(name)s - %(levelname)s - %(message)s')
    ch.setFormatter(formatter)
    log.addHandler(ch)

def get_color_space_specs(fmt):
    """
    Infer binary layout of the colorspace named by 'fmt' (see lcms2.h).

    That includes
        * the total number of channels (channels + extra channels)
        * bytes per channel (where 0 means 8 as used for doubles)
        * floating point or integer

    :param fmt: name of the colorspace in lcms2 specific format as string
    :return: tuple (total number of channels, bytes per channel, is floating point)
    """
    try:
        colorspace_number = getattr(_lcms2, fmt)
    except AttributeError:
        raise ValueError("color space format '{}' not found!\n".format(fmt) +
                         "If it is defined in lcms2.h it should be added to 'PyInit__lcms2' in '_lcms2.c")

    log.debug("colorspace fmt '{}' has binary repr. {:b}".format(fmt, colorspace_number))
    B =  colorspace_number & 0b0000000111                      # bytes per channel
    C = (colorspace_number & 0b0001111000)  >> 3               # number of channels
    E = (colorspace_number & 0b1110000000) >> 7                # extra channels
    A = (colorspace_number & 0b10000000000000000000000) >> 22  # is floating point number

    if B == 0:
        B = 8
    log.debug("colorspace fmt '{}', channels={}, bytes={}, isFloat={}".format(fmt, C + E, B, bool(A)))
    return C+E, B, bool(A)


def do_transform(in_prof, in_Py_buffer, in_fmt, out_prof, out_fmt, intent, dw_flags):
    in_C, in_B, in_F = get_color_space_specs(in_fmt)
    out_C, out_B, out_F = get_color_space_specs(out_fmt)

    num_pixels = len(in_Py_buffer) // in_C
    log.debug("{} pixels determined from input buffer on length {}".format(num_pixels, len(in_Py_buffer)))

    out_Py_buffer = bytearray(num_pixels *  out_C * out_B)
    log.debug("create output buffer with {} bytes".format(len(out_Py_buffer)))

    ret_code, error_msg = _lcms2.do_transform(
        in_prof,
        out_prof,
        in_Py_buffer,
        getattr(_lcms2, in_fmt),
        out_Py_buffer,
        getattr(_lcms2, out_fmt),
        intent,
        dw_flags,
        num_pixels
    )
    log.debug("calling 'do_transform' from lcms2 returned '{}'".format(ret_code))
    if ret_code != 0:
        raise RuntimeError("calling 'do_transform' from lcms2 failed with error '{}'".format(error_msg))

    return out_Py_buffer
