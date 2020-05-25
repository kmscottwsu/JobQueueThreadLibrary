#ifndef MUTAPHORE_H
#define MUTAPHORE_H

#include "StdLibraryDependencies.h"




class Mutaphore
{
public:
  Mutaphore();
  ~Mutaphore();

  void wait(bool bNeverWait = false);
  void notify_one();

  bool predicate();

private:
  int count_;
  std::mutex mut_;
  std::condition_variable cvMuta;
  bool bAlwaysWait_;
};

#endif