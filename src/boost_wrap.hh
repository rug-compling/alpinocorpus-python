#ifndef BOOST_WRAP_HH
#define BOOST_WRAP_HH

#include <functional>

#include "EntryIterator.hh"

// XXX - we need this, because Python headers conflict with Boost headers.

namespace boost {
  class thread;
}

class InterruptIter;

void sleepBoostThread(int timeout);
boost::thread *createBoostThread(InterruptIter const &callable);
void interruptBoostThread(boost::thread *thread);
void deleteBoostThread(boost::thread *thread);

struct InterruptIter
{
public:
  InterruptIter(EntryIterator *iter, int timeout)
    : d_iter(iter), d_timeout(timeout) {}

  void operator()() {
    sleepBoostThread(d_timeout);
    d_iter->iter->interrupt();
  }

private:
  EntryIterator *d_iter;
  int d_timeout;
};

#endif // BOOST_WRAP_HH
