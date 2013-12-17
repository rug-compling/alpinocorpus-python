from distutils.core import setup, Extension

alpinocorpus = Extension('alpinocorpus',
	libraries = ['alpino_corpus', 'exslt', 'boost_filesystem-mt',
      'boost_thread-mt'],
    sources = ['src/alpinocorpus.cpp', 'src/CorpusReader.cpp',
      'src/Entry.cpp', 'src/EntryIterator.cpp', 'src/Macros.cpp',
      'src/MarkerQuery.cpp', 'src/boost_wrap.cpp'],
    include_dirs = ['/usr/include/libxml2'])

setup (name = 'alpinocorpus',
       version = '1.0.0',
       description = 'Bindings for the alpinocorpus treebank library',
       ext_modules = [alpinocorpus])
