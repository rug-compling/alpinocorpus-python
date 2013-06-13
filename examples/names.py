#!/usr/bin/python

import alpinocorpus
import os.path
import sys

if __name__ == "__main__":
  if (len(sys.argv) != 2):
    print "%s: corpus" % sys.argv[0]
    sys.exit(1)

  scriptDir = os.path.dirname(sys.argv[0])
  scriptPath = os.path.join(scriptDir, "names.xq")
  script = open(scriptPath, 'r').read()

  reader = alpinocorpus.CorpusReader(sys.argv[1])

  for entry in reader.xquery(script):
    print entry.contents()

