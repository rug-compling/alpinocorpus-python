import alpinocorpus
import os
import ConfigParser

ini = ConfigParser.ConfigParser()
ini.read("corpora.ini")
corpora = {}
for corpus in ini.sections():
    corpora[corpus] = {}
    for name, value in ini.items(corpus):
        if name == "shortdesc" or name == "desc":
            corpora[corpus][name] = value.decode("utf-8")
        elif name == "path":
            corpora[corpus][name] = value

removes = list()

for name, info in corpora.iteritems():
    try:
        c = alpinocorpus.CorpusReader(info['path'])
        info['entries'] = c.size()
    except RuntimeError:
        removes.append(name)
        continue
    try:
        size = os.stat(info['path']).st_size
    except:
        size = 0
    info['filesize'] = size
    if not info.get('desc', ''):
        info['desc'] = info.get('shortdesc', u'')
    if not info.get('shortdesc', ''):
        info['shortdesc'] = info.get('desc', u'')
    i = info['shortdesc'].find(u'\n')
    if i > -1:
        info['shortdesc'] = info['shortdesc'][:i]

for r in removes:
    print "Could not open %s, removing corpus" % r
    del(corpora[r])
