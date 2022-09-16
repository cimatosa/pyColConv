import pylcms2

# pylcms2.useDebugMode()
import numpy as np

in_data = np.asarray([0, 0, 0], dtype="f8")
out_data = pylcms2.convert(
    in_prof=pylcms2.PROFILE_SRGB,
    in_Py_buffer=in_data.data,
    in_fmt="TYPE_RGB_DBL",
    out_prof="../data/PSOcoated_v3.icc",
    out_fmt="TYPE_CMYK_DBL",
    intent=pylcms2.INTENT_RELATIVE_COLORIMETRIC,
    dw_flags=pylcms2.cmsFLAGS_BLACKPOINTCOMPENSATION,
)

out_data = np.frombuffer(out_data, dtype="f8")
print("sRGB : {} -> (PSOcoated_v3) CMYK : {}".format(in_data, out_data))

in_data = out_data
out_data = pylcms2.convert(
    in_prof="../data/PSOcoated_v3.icc",
    in_Py_buffer=in_data.data,
    in_fmt="TYPE_CMYK_DBL",
    out_prof=pylcms2.PROFILE_Lab_D50,
    out_fmt="TYPE_Lab_DBL",
    intent=pylcms2.INTENT_ABSOLUTE_COLORIMETRIC,
    dw_flags=0,
)

out_data = np.frombuffer(out_data, dtype="f8")
print("(PSOcoated_v3) CMYK : {} -> Lab : {}".format(in_data, out_data))

in_data = np.asarray([44, 39, 37, 93], dtype="f8")
out_data = pylcms2.convert(
    in_prof="../data/PSOcoated_v3.icc",
    in_Py_buffer=in_data.data,
    in_fmt="TYPE_CMYK_DBL",
    out_prof=pylcms2.PROFILE_Lab_D50,
    out_fmt="TYPE_Lab_DBL",
    intent=pylcms2.INTENT_ABSOLUTE_COLORIMETRIC,
    dw_flags=0,
)

out_data = np.frombuffer(out_data, dtype="f8")
print("(PSOcoated_v3) CMYK : {} -> Lab : {}".format(in_data, out_data))
