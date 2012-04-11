#!/usr/bin/python

import alpinocorpus
from lxml import etree
import os.path
import sys

class Transformer:
  def __init__(self, stylesheet):
    self.doc = etree.parse(stylesheet)
    self.style = etree.XSLT(self.doc)

  def close(self):
    pass

  def transform(self, data):
    doc = etree.XML(data.encode("utf-8"))
    result = self.style(doc)
    return str(result).strip()

def matchAndPrint(stylesheet, reader, query):
  trans = Transformer(stylesheet)

  markerQueries = [alpinocorpus.MarkerQuery(query, "active", "1")]
  for entry in reader.query(query):
    print(trans.transform(reader.readMarkQueries(entry.name(), markerQueries)))

  trans.close()

if __name__ == "__main__":
  if (len(sys.argv) != 3):
    print("%s: query corpus" % sys.argv[0])
    sys.exit(1)

  styledir = os.path.dirname(sys.argv[0])
  stylesheet = os.path.join(styledir, "bracketed-sentence.xsl")

  query = sys.argv[1]
  reader = alpinocorpus.CorpusReader(sys.argv[2])

  matchAndPrint(stylesheet, reader, query)
