import alpinocorpus
import os
import ConfigParser

ini = ConfigParser.ConfigParser()
ini.read("corpora.ini")
corpora = {}
for corpus in ini.sections():
    corpora[corpus] = {}
    for name, value in ini.items(corpus):
        if name == "shortdesc" or name == "longdesc":
            corpora[corpus][name] = value.decode("utf-8")
        else:
            corpora[corpus][name] = value

removes = list()

for name, info in corpora.iteritems():
    try:
        size = os.stat(info['path']).st_size
    except:
        size = 0
    info['filesize'] = size
    try:
        c = alpinocorpus.CorpusReader(info['path'])
        info['entries'] = c.size()
    except RuntimeError:
        print "Could not open %s, removing corpus" % name
        removes.append(name)

for r in removes:
    print r
    del(corpora[r])
