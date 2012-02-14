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
# * /corpora                    : provides a list of corpora, tab separated values
# * /corpora.(js|json|xml)      : provides a list of corpora, javascript, json, xml
# * /<corpus>/entries           : provides a list of entries in the given corpus, tab separated values
# * /<corpus>/entries.(js|json) : provides a list of entries in the given corpus, javascript, json
# * /<corpus>/entry/<entry>     : retrieve an entry from a corpus
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

import json, web, sys

import alpinocorpus

from server_config import corpora

_timeout = 20

urls = (
    '/corpora(|\\.js|\\.json|\\.xml)', 'Corpora',
    '/([^/]*)/entries(|\\.js|\\.json)/?', 'Entries',
    '/([^/]*)/entry/(.*)', 'Entry',
    '/([^/]*)/validate', 'QueryValidation',
    '/([^/]*)/validQuery', 'QueryValidation'
)

app = web.application(urls, globals())

def escapeSpecials(txt):
    return txt.replace('\\', '\\\\').replace('\n', '\\n').replace('\r', '\\r').replace('\t', '\\t')

def escapeXML(txt):
    return txt.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;').encode('ascii', 'xmlcharrefreplace')

class Corpora:
    def GET(self, ext):

        if ext == '.xml':

            web.header('Content-Type', 'text/xml')

            yield '<corpusarchive>\n'
            for corpus in sorted(corpora):
                info = corpora[corpus]
                yield '''<corpus>
                    <filename>%s</filename>
                    <filesize>%d</filesize>
                    <sentences>%d</sentences>
                    <shortdesc>%s</shortdesc>
                    <desc>%s</desc>
                </corpus>
''' % (corpus, info['filesize'], info['entries'], escapeXML(info['shortdesc']), escapeXML(info['desc']))
            yield '</corpusarchive>\n'

        elif ext == '.js' or ext == '.json':

            if ext == '.js':
                mst = 'javascript'
            else:
                mst = 'json'
            web.header('Content-Type', 'application/%s' % mst)

            if ext == '.js':
                yield 'var ac_corpora = '
            c = {}
            for corpus, info in corpora.iteritems():
                c[corpus] = {}
                for i in info:
                    if i != 'path' and i != 'reader':
                        c[corpus][i] = info[i]
            yield json.dumps(c, sort_keys=True).replace('"}, "', '"},\n "')
            if ext == '.js':
                yield ';\n'
            else:
                yield '\n'

        else:

            web.header('Content-Type', 'text/plain; charset=utf-8')

            for corpus in sorted(corpora):
                info = corpora[corpus]
                yield '%s\t%d\t%s\t%s\n' % (corpus,
                                            info['entries'],
                                            escapeSpecials(info['shortdesc']).encode('utf-8'),
                                            escapeSpecials(info['desc']).encode('utf-8'))

class Entries:

    def run(self, gen, ext, contents, offset):

        if ext == '.js':
            web.header('Content-Type', 'application/javascript')
            yield 'var ac_data = {"entries": ['
        if ext == '.json':
            web.header('Content-Type', 'application/json')
            yield '{"entries": ['
        else:
            web.header('Content-Type', 'text/plain; charset=utf-8')
            yield '\002\n'

        pre = ''
        timeout = False
        while True:
            try:
                e = gen.next()
            except StopIteration:
                break
            except:
                timeout = True
                break
            if offset:
                offset -= 1
                continue
            if contents:
                if ext[:3] == '.js':
                    yield  pre + '\n    ' + json.dumps([e.name(), escapeSpecials(e.contents())])
                else:
                    yield '%s\t%s\n' % (e.name(), escapeSpecials(e.contents()))
            else:
                if ext[:3] == '.js':
                    yield pre + '\n    ' + json.dumps(e.name())
                else:
                    yield '%s\n' % e.name()
            pre = ','

        if ext == '.json':
            yield '\n  ],\n "timeout": %s\n}\n' % str(timeout).lower()
        elif ext == '.js':
            yield '\n  ],\n "timeout": %s\n};\n' % str(timeout).lower()
        else:
            if timeout:
                yield '[ Timeout ]\n'
            yield '\004\n'

    def GET(self, name, ext):
        if not corpora.has_key(name):
            yield web.notfound()
            return

        try:
            c = corpora[name]['reader']

            params = web.input()
            offset = int(params.get('start', '0'))

            # Was a query provided?
            contents = False
            if params.has_key('query'):
                gen = c.query(params['query'].encode('utf-8'), _timeout)
                if params.get('contents', '') == '1':
                    contents = True
            else:
                gen = c.entries()

            for i in self.run(gen, ext, contents, offset):
                yield i

        except:
            print sys.exc_info()
            yield web.internalerror()

    def POST(self, name, ext):
        if not corpora.has_key(name):
            yield web.notfound()
            return

        try:
            c = corpora[name]['reader']

            params = web.input()
            offset = int(params.get('start', '0'))

            # Do we want to highlight something?
            if params.has_key('markerQuery') and params.has_key('markerAttr') and params.has_key('markerValue'):
                markerQueries = [alpinocorpus.MarkerQuery(params['markerQuery'].encode('utf-8'),
                    params['markerAttr'], params['markerValue'])]
            else:
                markerQueries = []

            # Was a query provided?
            if params.has_key('query'):
                gen = c.queryWithStylesheet(params['query'].encode('utf-8'), web.data(), markerQueries, _timeout)
            else:
                gen = c.entriesWithStylesheet(web.data(), markerQueries, _timeout)

            for i in self.run(gen, ext, True, offset):
                yield i

        except:
            print sys.exc_info()
            yield web.internalerror()

class Entry:
    def GET(self, name, entry):
        web.header('Content-Type', 'text/xml')

        if not corpora.has_key(name):
            return web.notfound()

        params = web.input()
        try:
            c = corpora[name]['reader']

            # Was there a request to mark entries?
            if params.has_key('markerQuery') and params.has_key('markerAttr') and params.has_key('markerValue'):
                queries = [alpinocorpus.MarkerQuery(params['markerQuery'].encode('utf-8'),
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
            c = corpora[name]['reader']

            # Was there a request to mark entries?
            if params.has_key('query'):
                query = params.get('query').encode('utf-8')
                if c.validQuery(query):
                    return '1\n'
                else:
                    return '0\n'
            else:
                return web.notfound()

        except RuntimeError:
            return web.notfound()

if __name__ == '__main__':

    app.run()
