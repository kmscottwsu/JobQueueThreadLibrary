#include "JobQueue.h"




SubQueue::SubQueue(int i)
{
  iThreadId_ = i;
  pThreadSpecificJob_.store(NULL);
  iJobCount_.store(0);
  bQueueInUse_.clear();
}

SubQueue::~SubQueue()
{

}

void SubQueue::setThreadSpecificJob(Job* pJob)
{
  pThreadSpecificJob_.store( pJob);
}

bool SubQueue::hasThreadSpecificJob()
{
  if (pThreadSpecificJob_.load() != NULL) {
    return true;
  }
  else {
    return false;
  }
}

void SubQueue::addJob(std::function<void(JobOptions)> func, std::string name)
{
  while (bQueueInUse_.test_and_set()) {}
  jobQueue_.emplace(func, name);
  iJobCount_++;
  bQueueInUse_.clear();
}

void SubQueue::addJob(Job& job)
{
  while (bQueueInUse_.test_and_set()) {};//wait for any threads using queue to leave, technically a lock
  jobQueue_.push(job);
  iJobCount_++;
  bQueueInUse_.clear();
}



bool SubQueue::findJob(Job& job, int& index)
{
  if ( index == iThreadId_ ) {
    Job* pJob = pThreadSpecificJob_.load();//thread specific jobs are lockless
    if (pJob != NULL) {
      job = *pJob;//doesn't really matter if job has changed since load
      return true;
    }
  }
  if (iJobCount_.load() <= 0) {
    return false;//queue empty
  }
  else if(bQueueInUse_.test_and_set()) {//if true, already in use
    return false;
  }
  else {//was able to use the queue
    iJobCount_--;
    job = jobQueue_.front();
    jobQueue_.pop();
    bQueueInUse_.clear();
    return true;
  }
}

JobQueue::JobQueue()
{
  numThreads_ = 1;
  vSubQueues_.resize(numThreads_);
  vSubQueues_[0] = new SubQueue(0);
}

void JobQueue::resize(int& newSize)
{
  //this function is not thread safe, also doesn't work
  if (newSize > numThreads_) {
    vSubQueues_.resize(newSize);
    for (int i = numThreads_; i < newSize; ++i) {
      vSubQueues_[i] = new SubQueue(i);
    }
    numThreads_ = newSize;
  }
}

int JobQueue::size()
{
  return numThreads_;
}

bool JobQueue::hasThreadSpecificJob(int& i)
{
  return vSubQueues_[i]->hasThreadSpecificJob();
}


JobQueue::JobQueue(int& numThreads) :
  numThreads_(numThreads)
{
  vSubQueues_.resize(numThreads_);
  for (int i = 0; i < numThreads_; ++i) {
    vSubQueues_[i] = new SubQueue(i);
  }
}

JobQueue::~JobQueue()
{
  for (int i = 0; i < numThreads_; ++i) {
    delete vSubQueues_[i];
  }
}

void JobQueue::addJob(std::function<void(JobOptions)> func, int& index, std::string name)
{
  vSubQueues_[index]->addJob(func, name);
}

void JobQueue::addJob(Job& job, int& index)
{
  vSubQueues_[index]->addJob(job);
}

void JobQueue::setThreadSpecificJob(Job* pJob, int& index)
{
  vSubQueues_[index]->setThreadSpecificJob(pJob);
}

Job JobQueue::getJob(int& index)
{
  int i = index;
  if (i >= numThreads_) {
    i = 0;
  } else if (i < 0) {
    i = 0;
  }
  Job job;
  while (!vSubQueues_[i]->findJob(job, index))
  {
    ++i;
    if (i >= numThreads_) {
      i = 0;
    }
  }
  return job;
}