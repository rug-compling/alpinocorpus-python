import httplib
import urllib
import sys

stylesheet_path = "bracketed-sentence.xsl"

stylesheet = ""

with open(stylesheet_path) as f:
	stylesheet = f.read()

url = "/cdb/entries/?" + urllib.urlencode({
	"query": "//node[@rel='su']",
	"markerQuery": "//node[@rel='su']",
	"markerAttr": "active",
	"markerValue": "1"
})

conn = httplib.HTTPConnection("localhost", 8080)
conn.request("POST", url, stylesheet)

response = conn.getresponse()

print response.status, response.reason

while True:
  data = response.read(20)
  if len(data) == 0:
    break

  sys.stdout.write(data)

