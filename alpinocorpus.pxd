cdef extern from "AlpinoCorpus/capi.h":

    ctypedef struct marker_query_t:
        const char *query
        const char *attr
        const char *value

    ctypedef struct alpinocorpus_entry_t:
        pass
    ctypedef struct alpinocorpus_reader_t:
        pass
    ctypedef struct alpinocorpus_writer_t:
        pass
    ctypedef struct alpinocorpus_iter_t:
        pass

    ctypedef alpinocorpus_entry_t *alpinocorpus_entry
    ctypedef alpinocorpus_reader_t *alpinocorpus_reader
    ctypedef alpinocorpus_writer_t *alpinocorpus_writer
    ctypedef alpinocorpus_iter_t *alpinocorpus_iter

    void alpinocorpus_initialize()
    alpinocorpus_reader alpinocorpus_open(const char *path)
    alpinocorpus_reader alpinocorpus_open_recursive(const char *path)
    void alpinocorpus_close(alpinocorpus_reader corpus)
    char *alpinocorpus_read(alpinocorpus_reader corpus, const char *entry)
    alpinocorpus_iter alpinocorpus_entry_iter(alpinocorpus_reader corpus)
    void alpinocorpus_iter_destroy(alpinocorpus_iter iter)
    int alpinocorpus_iter_has_next(alpinocorpus_reader corpus, alpinocorpus_iter iter)
    alpinocorpus_entry alpinocorpus_iter_next(alpinocorpus_reader corpus, alpinocorpus_iter iter)
    const char *alpinocorpus_entry_name(alpinocorpus_entry entry)
    void alpinocorpus_entry_free(alpinocorpus_entry entry)
    size_t alpinocorpus_size(alpinocorpus_reader corpus)
    const char * alpinocorpus_entry_contents(alpinocorpus_entry entry)
    int alpinocorpus_is_valid_query(alpinocorpus_reader corpus, const char *query)
    char *alpinocorpus_name(alpinocorpus_reader corpus)
    alpinocorpus_iter alpinocorpus_query_iter(alpinocorpus_reader reader, const char *query)
    alpinocorpus_iter alpinocorpus_query_stylesheet_iter(alpinocorpus_reader corpus, const char *query, const char *stylesheet, marker_query_t *queries, size_t n_queries)
    char *alpinocorpus_read_mark_queries(alpinocorpus_reader reader, const char *entry, marker_query_t *queries, size_t n_queries)
    int alpinocorpus_writer_available(const char *writertype)
    alpinocorpus_writer alpinocorpus_writer_open(const char *path, int overwrite, const char *writertype)
    void alpinocorpus_writer_close(alpinocorpus_writer)
    char *alpinocorpus_write(alpinocorpus_writer, const char *name, const char *content)
    char *alpinocorpus_write_corpus(alpinocorpus_writer, alpinocorpus_reader, int failsafe)
