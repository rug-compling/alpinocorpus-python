# -*- coding: utf-8 -*-

from distutils.core import setup, Extension

alpinocorpus = Extension('alpinocorpus',
    libraries = ['alpino_corpus', 'exslt', 'boost_filesystem',
      'boost_thread'],
    sources = ['src/alpinocorpus.cpp', 'src/CorpusReader.cpp',
      'src/Entry.cpp', 'src/EntryIterator.cpp', 'src/MarkerQuery.cpp',
      'src/boost_wrap.cpp'],
    include_dirs = ['/usr/include/libxml2'])

setup (name = 'alpinocorpus',
       version = '1.0.0',
       description = 'Bindings for the alpinocorpus treebank library',
       author = "Daniël de Kok",
       author_email = "me@danieldk.eu",
       url = "http://rug-compling.github.com/alpinocorpus",
       long_description = '''
This module provides bindings for the alpinocorpus treebank library.
Alpinocorpus provides access to Alpino treebanks.
''',
       ext_modules = [alpinocorpus])
