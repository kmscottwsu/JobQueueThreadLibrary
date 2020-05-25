#include "BusyWorkManager.h"


BusyWorkManager::BusyWorkManager(JobQueueManager* pJobQueueManager)
{
  pJobQueueManager_ = pJobQueueManager;
  i_ = 0;
}

BusyWorkManager::~BusyWorkManager() {
  std::cout << "job count: " << i_ << std::endl;
}

void BusyWorkManager::busyWork(JobOptions options) {
  double j = 0;
  for (int i = 0; i < 100000; ++i) {
    j = rand();
  }
  pJobQueueManager_->addJob(&BusyWorkManager::busyWork, this, options.myThreadId_, "BusyWork");
  std::unique_lock<std::mutex> lock(mut_);
  ++i_;
}

void BusyWorkManager::killThreadDelay(JobOptions options) {
  using namespace std::chrono_literals;
  //std::this_thread::sleep_for(30s);

  pJobQueueManager_->killThreads();
}