from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules=[
    Extension("alpinocorpus",
              ["alpinocorpus.pyx"],
              libraries=["alpino_corpus"]) 
]

setup(
    name = "alpinocorpus",
    cmdclass = {"build_ext": build_ext},
    ext_modules = ext_modules
)
