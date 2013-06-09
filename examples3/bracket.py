#!/usr/bin/python

import alpinocorpus
import os.path
import sys

def matchAndPrint(stylesheet, reader, query):
  markerQueries = [alpinocorpus.MarkerQuery(query, "active", "1")]
  for entry in reader.xpathWithStylesheet(query, stylesheet, markerQueries):
    print(entry.contents())

if __name__ == "__main__":
  if (len(sys.argv) != 3):
    print("%s: query corpus" % sys.argv[0])
    sys.exit(1)

  styledir = os.path.dirname(sys.argv[0])
  stylesheetPath = os.path.join(styledir, "bracketed-sentence.xsl")
  stylesheet = open(stylesheetPath, 'r').read()

  query = sys.argv[1]
  reader = alpinocorpus.CorpusReader(sys.argv[2])

  matchAndPrint(stylesheet, reader, query)
