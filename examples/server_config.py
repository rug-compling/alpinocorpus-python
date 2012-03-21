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

for name, corpusData in corpora.iteritems():
    print "Opening", corpusData['path']
    try:
        c = alpinocorpus.CorpusReader(corpusData['path'])
        corpusData['entries'] = c.size()
        corpusData['reader'] = c
    except RuntimeError:
        removes.append(name)
        continue
    try:
        if corpusData['path'].endswith('/'):
            size = 0
            for filename in os.listdir(corpusData['path']):
                if filename.endswith('.dact') or filename.endswith('.index') or filename.endswith('.data.dz'):
                    size += os.stat(corpusData['path'] + filename).st_size
        else:
            size = os.stat(corpusData['path']).st_size
    except:
        size = 0
    corpusData['filesize'] = size
    if not corpusData.get('desc', ''):
        corpusData['desc'] = corpusData.get('shortdesc', u'')
    if not corpusData.get('shortdesc', ''):
        corpusData['shortdesc'] = coprusData.get('desc', u'')
    i = corpusData['shortdesc'].find(u'\n')
    if i > -1:
        corpusData['shortdesc'] = corpusData['shortdesc'][:i]

for r in removes:
    print "Could not open %s, removing corpus" % r
    del(corpora[r])
