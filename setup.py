from setuptools import Extension, setup

setup(
    # Name of your Package
    name="pyColConv",
    # Project Version
    version="0.1",
    # Description of your Package
    description="implement color conversion using Little CMS for python3",
    # Website for your Project or Github repo
    url="https://github.com/cimatosa/pyColConv",
    # Name of the Creator
    author="Richard Hartmann",
    # Dependencies/Other modules required for your package to work
    install_requires=[],
    # Detailed description of your package
    long_description=(
        "The pyColConv package provides a simple pythonic way to convert "
        + "color between color spaces. It is based on the color management engine "
        + "Little CMS (www.littlecms.com)."
    ),
    # Format of your Detailed Description
    long_description_content_type="text/markdown",
    # Classifiers allow your Package to be categorized based on functionality
    classifiers=[
        "Programming Language :: Python :: 3",
        "Topic :: Multimedia :: Graphics",
        "Topic :: Scientific/Engineering :: Image Processing",
    ],
    ext_modules=[
        Extension(
            name="_lcms2",  # as it would be imported
            # may include packages/namespaces separated by `.`
            sources=[
                "./pyColConv/_lcms2.c"
            ],  # all sources are compiled into a single binary file
            libraries=["lcms2"],
            extra_compile_args=["-std=gnu99"],
        ),
    ],
)
