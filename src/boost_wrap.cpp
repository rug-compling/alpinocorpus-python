#include <boost/thread/xtime.hpp>
#include <boost/thread/thread.hpp>

#include "boost_wrap.hh"

boost::thread *createBoostThread(InterruptIter const &callable)
{
  return new boost::thread(callable);
}

void interruptBoostThread(boost::thread *thread)
{
  thread->interrupt();
  thread->join();
}

void deleteBoostThread(boost::thread *thread)
{
  delete thread;
}

void sleepBoostThread(int timeout)
{
  boost::xtime xt;
  boost::xtime_get(&xt, boost::TIME_UTC);
  xt.sec += timeout;

  boost::thread::sleep(xt);
}
