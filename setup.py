from distutils.core import setup, Extension

alpinocorpus = Extension('alpinocorpus',
	libraries = ['alpino_corpus'],
    sources = ['src/alpinocorpus.cpp'])

setup (name = 'alpinocorpus',
       version = '1.0.0',
       description = 'Bindings for the alpinocorpus treebank library',
       ext_modules = [alpinocorpus])
