# -*- coding: utf-8 -*-
import alpinocorpus

corpora = { 
    "cdb": {
      "path": "/Users/daniel/Desktop/cdb.dact",
      "shortDesc": u"Eindhoven Corpus",
      "longDesc": u"Eindhoven Corpus"
    },
    "lassy-small": {
      'path': "/Users/daniel/Desktop/lassy.dact",
      'shortDesc': u'Lassy Small',
      'longDesc': u'Lassy Small'
    }
}

removes = list()

for name, info in corpora.iteritems():
  try:
    c = alpinocorpus.CorpusReader(info['path'])
    info['entries'] = c.size()
  except RuntimeError:
    print "Could not open %s, removing corpus" % name
    removes.append(name)

for r in removes:
  print r
  del(corpora[r])
