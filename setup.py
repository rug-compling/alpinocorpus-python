from distutils.core import setup, Extension

alpinocorpus = Extension('alpinocorpus',
	libraries = ['alpino_corpus', 'exslt', 'boost_filesystem',
      'boost_thread'],
    sources = ['src/alpinocorpus.cpp', 'src/CorpusReader.cpp',
      'src/CorpusWriter.cpp', 'src/Entry.cpp', 'src/EntryIterator.cpp',
      'src/Macros.cpp', 'src/MarkerQuery.cpp', 'src/boost_wrap.cpp'],
    include_dirs = ['/usr/include/libxml2'],
    extra_compile_args=['-std=c++11'])

setup (name = 'alpinocorpus',
       version = '1.0.0',
       description = 'Bindings for the alpinocorpus treebank library',
       ext_modules = [alpinocorpus])
