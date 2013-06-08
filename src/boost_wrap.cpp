#include <boost/version.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>

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
#if BOOST_VERSION < 105000
  boost::xtime_get(&xt, boost::TIME_UTC);
#else
  boost::xtime_get(&xt, boost::TIME_UTC_);
#endif
  xt.sec += timeout;

  boost::thread::sleep(xt);
}

bool isDirectory(std::string const &path)
{
  boost::filesystem::path p(path);
  return boost::filesystem::is_directory(p);
}
