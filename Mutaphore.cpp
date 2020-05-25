#include "mutaphore.h"

Mutaphore::Mutaphore()
{
  count_ = 0;
  bAlwaysWait_ = false;
}

Mutaphore::~Mutaphore()
{

}


void Mutaphore::wait(bool bNeverWait)
{
  //bSem defaults to true
  std::unique_lock<std::mutex> lock(mut_);
  if (!bNeverWait) {
    if ((count_ <= 0) || (bAlwaysWait_)) {
      cvMuta.wait(lock, [this]() { return (count_ > 0); });
    }
    --count_;
  }
}

void Mutaphore::notify_one()
{
  std::unique_lock<std::mutex> lock(mut_);
  ++count_;
  lock.unlock();
  cvMuta.notify_one();
}

bool Mutaphore::predicate()
{
  return (count_ > 0);
}