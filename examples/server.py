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
      '/corpora', 'Corpora',
      '/([^/]*)/entries/?', 'Entries',
      '/(.*)/entry/(.*)', 'Entry',
      '/(.*)/validate', 'QueryValidation'
)

corpora = { 
    "cdb": {
      "path": "/Users/daniel/Desktop/cdb.dact",
      "shortDesc": "Eindhoven Corpus",
      "longDesc": "Eindhoven Corpus"
    },
    "lassy-small": {
      'path': "/Users/daniel/Desktop/lassy.dact",
      'shortDesc': 'Lassy Small',
      'longDesc': 'Lassy Small'
    }
}

app = web.application(urls, globals())

class Corpora:
  def GET(self):
    for corpus, info in corpora.iteritems():
      yield "%s\t%d\t%s\t%s\n" % (corpus, -1, info['shortDesc'], info['longDesc'])

class Entries:
  def GET(self, name):
    web.header('Content-Type', 'text/plain') 
    web.header('Transfer-Encoding', 'chunked') 

    if not corpora.has_key(name):
      yield web.notfound()

    try:
      c = alpinocorpus.CorpusReader(corpora[name]['path'])

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
      c = alpinocorpus.CorpusReader(corpora[name]['path'])

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

class QueryValidation:
  def GET(self, name):
    web.header('Content-Type', 'text/plain')

    if not corpora.has_key(name):
      return web.notfound()

    params = web.input()
    try:
      c = alpinocorpus.CorpusReader(corpora[name]['path'])

      # Was there a request to mark entries?
      if params.has_key('query'):
        query = params.get('query')
        if c.validQuery(query):
          return '1'
        else:
          return '0'
      else:
        return web.notfound()

    except RuntimeError:
      return web.notfound()    

if __name__ == "__main__":
  app.run()
