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
  for (int i = 0; i < 1000; ++i) {
    j = rand();
  }
  pJobQueueManager_->addJob(&BusyWorkManager::busyWork, this, options.myThreadId_, "BusyWork");
}

void BusyWorkManager::killThreadDelay(JobOptions options) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(30s);

  pJobQueueManager_->killThreads();
}