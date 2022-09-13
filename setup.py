from setuptools import Extension, setup

setup(
    # Name of your Package
    name='pylcms2',

    # Project Version
    version='0.1',

    # Description of your Package
    description='make little CMS 2 available for python3',

    # Website for your Project or Github repo
    url="https://github.com/cimatosa/pylcms2",

    # Name of the Creator
    author='Richard Hartmann',

    # Dependencies/Other modules required for your package to work
    install_requires=[],

    # Detailed description of your package
    long_description='',

    # Format of your Detailed Description
    long_description_content_type="text/markdown",

    # Classifiers allow your Package to be categorized based on functionality
    classifiers=[
        "Programming Language :: Python :: 3",
        "Topic :: Multimedia :: Graphics",
        "Topic :: Scientific/Engineering :: Image Processing"
    ],

    ext_modules=[
        Extension(
            name="pylcms2",  # as it would be imported
                             # may include packages/namespaces separated by `.`
            sources=["./src/_pylcms2.c"], # all sources are compiled into a single binary file
            libraries = ['lcms2'],
            extra_compile_args = ['-std=gnu99']
        ),
    ]
)
