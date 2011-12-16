#!/usr/bin/python
# -*- coding: utf-8 -*-

# server.py - A simple web API for Alpino corpora
#
# Author: Daniël de Kok <me@danieldk.eu>
#
# This is a simple web service that provides an interface to Alpino corpora
# using the alpinocorpus Python module. The only requirement is the web.py
# lightweight web framework.
#
# The API of this server is pretty simple: the following URLS are used:
#
# * /                       : provides a list of corpora
# * /<corpus>/entries       : provides a list of entries in the given corpus
# * /<corpus>/entry/<entry> : retrieve an entry from a corpus
#
# The list of entries can be filtered by providing the 'query' parameter with
# an XPath query.
#
# Nodes matching an XPath query can be marked when retrieving an entry, by
# specifying the following parameters:
#
# * markerQuery : the query to use for marking nodes
# * markerAttr  : marker attribute
# * markerValue : marker value

import web

import alpinocorpus

urls = (
      '/', 'Index',
      '/([^/]*)/entries/?', 'Entries',
      '/(.*)/entry/(.*)', 'Entry'
)

corpora = { 
    "cdb": "/Users/daniel/Desktop/cdb.dact",
}

app = web.application(urls, globals())

class Index:
  def GET(self):
    return " ".join(corpora.keys())

class Entries:
  def GET(self, name):
    web.header('Content-Type', 'text/plain') 
    web.header('Transfer-Encoding', 'chunked') 

    if not corpora.has_key(name):
      yield web.notfound()

    try:
      c = alpinocorpus.CorpusReader(corpora[name])

      # Was a query provided?
      params = web.input()
      contents = False
      if params.has_key('query'):
        gen = c.query(params['query'])
        if params.has_key('contents') and params.get('contents') == '1':
          contents = True
      else:
        gen = c.entries()

      # Stream (matching) entries
      for e in gen:
        if contents:
          yield "%s\t%s\n" % (e.name(), e.contents())
        else:
          yield "%s\n" % e.name()

    except RuntimeError:
      yield web.internalerror()

class Entry:
  def GET(self, name, entry):
    web.header('Content-Type', 'text/xml') 

    if not corpora.has_key(name):
      return web.notfound()

    params = web.input()
    try:
      c = alpinocorpus.CorpusReader(corpora[name])

      # Was there a request to mark entries?
      if params.has_key('markerQuery') and params.has_key('markerAttr') and params.has_key('markerValue'):
        queries = [alpinocorpus.MarkerQuery(params['markerQuery'],
          params['markerAttr'], params['markerValue'])]
        data = c.readMarkQueries(entry, queries)
      else:
        data = c.read(entry)

    except RuntimeError:
      return web.notfound()

    return data

if __name__ == "__main__":
  app.run()
