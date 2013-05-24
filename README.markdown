# alpinocorpus-python

This module provides Python bindings for the alpinocorpus library. The
bindings work for Python 2.x and 3.x.

Bugs/questions? Please send them to Daniël de Kok &lt;me@danieldk.eu&gt;

## Compilation

The binding is installed in three simple steps:

~~~
python setup.py config
python setup.py build
python setup.py install
~~~

## Usage

The Python binding for alpinocorpus can be loaded with a simple import:

    import alpinocorpus

The *CorpusReader* class is used to read and query corpora. A corpus
is opened using the constructor of *CorpusReader*:

    c = CorpusReader("mycorpus.dact")

A query can be executed using the *query* method:

    q = c.query("//node[@root='loop']")

The resulting query object is iterable, and can be used to print the
name or contents of a match:

    for entry in q:
      print(q.name())
      print(q.contents())

An entry can be read from the corpus using the *read* method of
*CorpusReader*:

    c.read("1.xml")

Or if you want to read, while marking nodes that are selected using a
query:

    c.readMarkQueries("1.xml", [alpinocorpus.MarkerQuery(query, "active", "1")]

## Server

The server in examples/ requires the web.py framework, which can be
installed with:

~~~
easy_install web.py
~~~
