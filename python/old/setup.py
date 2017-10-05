'''

To build the extension:

python setup.py build_ext --inplace

'''

from distutils.core import setup
from distutils.extension import Extension

from Cython.Distutils import build_ext
from Cython.Build import cythonize

extensions = [
    Extension(
            'pyfxis',
            sources=['pyfxis.pyx', '../src/TimingExperiments.cpp'],
            language="c++",
            extra_compile_args=["-std=c++14"],
            extra_link_args=["-std=c++14", "-static"],
            include_dirs=['../include', '/opt/Vimba_2_1', '/opt/Vimba_2_1/VimbaCPP/Examples'],
            libraries=['VimbaCPP'],
            library_dirs=['/opt/Vimba_2_1/VimbaCPP/DynamicLib/x86_64bit']
            #extra_compile_args=["-std=c++14", "-I../include", "-I/opt/Vimba_2_1", "-I/opt/Vimba_2_1/VimbaCPP/Examples"],
            #extra_link_args=["-std=c++14", "-L/opt/Vimba_2_1/VimbaCPP/DynamicLib/x86_64bit", "-lVimbaCPP"]
        )
    ]

setup(
    cmdclass={'buid_ext': build_ext},
    ext_modules=cythonize(
        extensions
    )
)
