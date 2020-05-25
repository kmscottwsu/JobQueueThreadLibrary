#include "JobQueue.h"

SubQueue::SubQueue(int i)
{
  iThreadId_ = i;
}

SubQueue::~SubQueue()
{

}

void SubQueue::setThreadSpecificJob(std::function<void(JobOptions)> func, std::string name)
{
  std::unique_lock<std::mutex> lock(mut_);
  jobThreadSpecific_ = Job(func);
}

void SubQueue::setThreadSpecificJob(Job& job)
{
  std::unique_lock<std::mutex> lock(mut_);
  jobThreadSpecific_ = job;
}

bool SubQueue::hasThreadSpecificJob()
{
  if (jobThreadSpecific_.isValid()) {
    return true;
  }
  else {
    return false;
  }
}


void SubQueue::addJob(std::function<void(JobOptions)> func, std::string name)
{
  std::unique_lock<std::mutex> lock(mut_);
  queue_.emplace(func, name);
}

void SubQueue::addJob(Job& job)
{
  std::unique_lock<std::mutex> lock(mut_);
  queue_.push(job);
}

bool SubQueue::findJob(Job& job, int& index)
{
  std::unique_lock<std::mutex> lock(mut_);
  if (( index == iThreadId_ ) && ( jobThreadSpecific_.isValid() )) {
    job = jobThreadSpecific_;
    return true;
  }
  else if (queue_.empty()) {
    return false;
  } 
  else {
    while (!queue_.front().isValid()) { queue_.pop(); }//remove invalid jobs
    job = queue_.front();
    queue_.pop();
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

void JobQueue::setThreadSpecificJob(std::function<void(JobOptions)> func, int& index)
{
  vSubQueues_[index]->setThreadSpecificJob(func);
}

void JobQueue::setThreadSpecificJob(Job& job, int& index)
{
  vSubQueues_[index]->setThreadSpecificJob(job);
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