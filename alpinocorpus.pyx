
#| doc

"""
Reader and writer for Alpino corpora

Usage examples for reader:

# open corpus, process all data, and close corpus:
with AlpinoCorpus.Reader(filename) as crps:
    for key, value in crps.items():
        print(key, '=', value)

# open a corpus for reading:
crps = AlpinoCorpus.Reader(filename)

# get names for all entries in the corpus:
for name in crps:
    print(name)

# another way to get names for all entries in the corpus:
for name in crps.keys():
    print(name)

# get data for all entries in the corpus:
for data in crps.values():
    print(data)

# get data for entry with specific name:
print(crps[name])

# get the name of the corpus
print(crps.name())

# get number of entries in the corpus
print(len(crps))

# is a query valid?
print(crps.valid_query(query))

# print results for query
for entry in crps.query(query):
    print(entry, '=', crps[entry])

# transform results from query using stylesheet (query is optional)
for entry, value in crps.query(query=query,
                               stylesheet=stylesheet,
                               markerQuery=query,
                               markerAttr='active',
                               markerValue='1'):
    print(entry, '=', value)

# mark some elements in entry
print(crps.read_mark_query(entryname, query, 'active', '1'))

# close the corpus
crps.close()
"""

__author__ = 'Peter Kleiweg'
__version__ = '0.1'
__date__ = '2012/03/24'

#| imports

cimport alpinocorpus
cimport libc.stdlib

from os import stat as _stat
from os import listdir as _listdir
from stat import S_ISDIR as _isdir

#| classes

cdef class Reader:

    cdef alpinocorpus.alpinocorpus_reader _c
    cdef alpinocorpus.alpinocorpus_iter _iter
    cdef _opened

    def __init__(self, corpusname):
        self._opened = False
        self._iter = NULL

        cname = corpusname.encode("utf-8")

        recursive = False
        try:
            assert _isdir(_stat(corpusname).st_mode)
        except:
            pass
        else:
            for i in _listdir(corpusname):
                if i.endswith('.xml'):
                    break
                if i.endswith('.dact'):
                    recursive = True
                    break
                if i.endswith('.data.dz'):
                    recursive = True
                    break

        if recursive:
            self._c = alpinocorpus.alpinocorpus_open_recursive(cname)
        else:
            self._c = alpinocorpus.alpinocorpus_open(cname)
        if not self._c:
            raise RuntimeError('Unable to open corpus: ' + corpusname)

        self._opened = True

    def __del__(self):
        self._close()

    def __dealloc__(self):
        self._close()

    def __enter__(self):
        self._isopen()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self._isopen()
        self._close()

    def _isopen(self):
        if not self._opened:
            raise RuntimeError('Corpus is closed')

    def _close(self):
        if self._opened:
            self._opened = False
            if self._iter != NULL:
                alpinocorpus.alpinocorpus_iter_destroy(self._iter)
                self._iter = NULL
            alpinocorpus.alpinocorpus_close(self._c)

    def __iter__(self):
        self._isopen()
        if self._iter != NULL:
            alpinocorpus.alpinocorpus_iter_destroy(self._iter)
        self._iter = alpinocorpus.alpinocorpus_entry_iter(self._c)
        return self

    def __next__(self):
        self._isopen()
        if self._iter != NULL:
            if not alpinocorpus.alpinocorpus_iter_has_next(self._c, self._iter):
                alpinocorpus.alpinocorpus_iter_destroy(self._iter)
                self._iter = NULL
        if self._iter == NULL:
            raise StopIteration
        ent = alpinocorpus.alpinocorpus_iter_next(self._c, self._iter)
        key = alpinocorpus.alpinocorpus_entry_name(ent)
        value = key.decode()
        alpinocorpus.alpinocorpus_entry_free(ent)
        return value

    def __getitem__(self, entry):
        self._isopen()
        e = entry.encode("utf-8")
        s = alpinocorpus.alpinocorpus_read(self._c, e)
        if not s:
            raise KeyError(entry)
        p = s + b''
        libc.stdlib.free(s)
        return p

    def __len__(self):
        self._isopen()
        return alpinocorpus.alpinocorpus_size(self._c)

    def close(self):
        self._close()

    def keys(self):
        self._isopen()
        it = alpinocorpus.alpinocorpus_entry_iter(self._c)
        while alpinocorpus.alpinocorpus_iter_has_next(self._c, it):
            ent = alpinocorpus.alpinocorpus_iter_next(self._c, it)
            k = alpinocorpus.alpinocorpus_entry_name(ent)
            key = k.decode()
            alpinocorpus.alpinocorpus_entry_free(ent)
            yield key
        alpinocorpus.alpinocorpus_iter_destroy(it)

    def values(self):
        self._isopen()
        it = alpinocorpus.alpinocorpus_entry_iter(self._c)
        while alpinocorpus.alpinocorpus_iter_has_next(self._c, it):
            ent = alpinocorpus.alpinocorpus_iter_next(self._c, it)
            val = alpinocorpus.alpinocorpus_entry_contents(ent)
            value = val + b''
            alpinocorpus.alpinocorpus_entry_free(ent)
            yield value
        alpinocorpus.alpinocorpus_iter_destroy(it)

    def items(self):
        self._isopen()
        it = alpinocorpus.alpinocorpus_entry_iter(self._c)
        while  alpinocorpus.alpinocorpus_iter_has_next(self._c, it):
            ent = alpinocorpus.alpinocorpus_iter_next(self._c, it)
            k = alpinocorpus.alpinocorpus_entry_name(ent)
            val = alpinocorpus.alpinocorpus_entry_contents(ent)
            key = k.decode()
            value = val + b''
            alpinocorpus.alpinocorpus_entry_free(ent)
            yield key, value
        alpinocorpus.alpinocorpus_iter_destroy(it)

    def valid_query(self, query):
        self._isopen()
        q = query.encode('utf-8')
        if alpinocorpus.alpinocorpus_is_valid_query(self._c, q):
            return True
        return False

    def name(self):
        self._isopen()
        s = alpinocorpus.alpinocorpus_name(self._c)
        p = s.decode()
        libc.stdlib.free(s)
        return p

    def query(self, query='', markerQuery='', markerAttr='', markerValue='', stylesheet=''):
        cdef marker_query_t[1] mq
        cdef _Iterator it
        cdef _Iterator2 it2
        
        self._isopen()
    
        marker = (markerQuery and markerAttr and markerValue)
        
        if query:
            if not self.valid_query(query):
                raise RuntimeError("Invalid query: " + query)

        if marker:
            if not self.valid_query(markerQuery):
                raise RuntimeError("Invalid marker query: " + markerQuery)
            mqq = markerQuery.encode('utf-8')
            mqa = markerAttr.encode('utf-8')
            mqv = markerValue.encode('utf-8')
            mq[0].query = mqq
            mq[0].attr = mqa
            mq[0].value = mqv
            mn = 1
        else:
            mn = 0

        q = query.encode('utf-8')
        if not (query or stylesheet):
            return self.__iter__()
        if not stylesheet:
            it = _Iterator()
            it._c = self._c
            it._iter = alpinocorpus.alpinocorpus_query_iter(self._c, q)
            return it

        it2 = _Iterator2()
        it2._c = self._c
        it2._iter = alpinocorpus.alpinocorpus_query_stylesheet_iter(self._c, q, stylesheet, mq, mn)
        return it2

    def read_mark_query(self, entry, markerQuery, markerAttr, markerValue):
        cdef marker_query_t[1] mq

        self._isopen()

        if not self.valid_query(markerQuery):
            raise RuntimeError("Invalid marker query: " + markerQuery)
        mqq = markerQuery.encode('utf-8')
        mq[0].query = mqq
        mq[0].attr = markerAttr
        mq[0].value = markerValue
        s = alpinocorpus.alpinocorpus_read_mark_queries(self._c, entry, mq, 1)
        if not s:
            raise KeyError(entry)
        p = s + b''
        libc.stdlib.free(s)
        return p


cdef class _Iterator:

    cdef alpinocorpus.alpinocorpus_reader _c
    cdef alpinocorpus.alpinocorpus_iter _iter

    def __del__(self):
        alpinocorpus.alpinocorpus_iter_destroy(self._iter)
        
    def __iter__(self):
        return self

    def __next__(self):
        if not alpinocorpus.alpinocorpus_iter_has_next(self._c, self._iter):
            raise StopIteration
        ent = alpinocorpus.alpinocorpus_iter_next(self._c, self._iter)
        k = alpinocorpus.alpinocorpus_entry_name(ent)
        key = k.decode()
        alpinocorpus.alpinocorpus_entry_free(ent)
        return key


cdef class _Iterator2:

    cdef alpinocorpus.alpinocorpus_reader _c
    cdef alpinocorpus.alpinocorpus_iter _iter

    def __del__(self):
        alpinocorpus.alpinocorpus_iter_destroy(self._iter)
            
    def __iter__(self):
        return self
    
    def __next__(self):
        if not alpinocorpus.alpinocorpus_iter_has_next(self._c, self._iter):
            raise StopIteration
        ent = alpinocorpus.alpinocorpus_iter_next(self._c, self._iter)
        k = alpinocorpus.alpinocorpus_entry_name(ent)
        val = alpinocorpus.alpinocorpus_entry_contents(ent)
        key = k.decode()
        value = val + b''
        alpinocorpus.alpinocorpus_entry_free(ent)
        return key, value


cdef class Writer:

    cdef alpinocorpus.alpinocorpus_writer _c
    cdef _opened

    def __init__(self, corpusname, overwrite = False, writertype = 'DBXML_CORPUS_WRITER'):
        
        self._opened = False
        cname = corpusname.encode("utf-8")
        if overwrite:
            ow = 1
        else:
            ow = 0
        wt = writertype.encode("utf-8")
        self._c = alpinocorpus.alpinocorpus_writer_open(cname, ow, wt)
        if not self._c:
            raise RuntimeError('Unable to open corpus: ' + corpusname)
        self._opened = True

    def __del__(self):
        self._close()

    def __enter__(self):
        self._isopen()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self._isopen()
        self._close()

    def _isopen(self):
        if not self._opened:
            raise RuntimeError('Corpus is closed')

    def _close(self):
        if self._opened:
            self._opened = False
            alpinocorpus.alpinocorpus_writer_close(self._c)

    def close(self):
        self._close()

    def write(self, name, content):
        self._isopen()
        n = name.encode()
        s = alpinocorpus.alpinocorpus_write(self._c, n, content);
        if s:
            e = s.decode()
            libc.stdlib.free(s)
            raise RuntimeError(e)

    def write_corpus(self, Reader reader, failsafe = False):
        self._isopen()
        if failsafe:
            fs = 1
        else:
            fs = 0
        s = alpinocorpus.alpinocorpus_write_corpus(self._c, reader._c, fs);
        if s:
            e = s.decode()
            libc.stdlib.free(s)
            raise RuntimeError(e)


#| functions

def WriterAvailable(writertype):
    ''' 
    Check whether a particular writer type is available.
    Currently, the only valid types are "COMPACT_CORPUS_WRITER" and "DBXML_CORPUS_WRITER".
    '''
    w = writertype.encode()
    if alpinocorpus.alpinocorpus_writer_available(w):
        return True
    return False


#| init

alpinocorpus.alpinocorpus_initialize()

