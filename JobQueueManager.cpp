#include "JobQueueManager.h"



JobQueueManager::JobQueueManager() :
  //iThreads_(std::thread::hardware_concurrency() * 2),
  iThreads_(std::thread::hardware_concurrency() + 1),//1 for waiting thread
  jobQueue_(iThreads_)//number of threads has to be handed in before construction
{
  time_ = std::chrono::high_resolution_clock::now();



  workers_ = 0;

  if (iThreads_ == 0) {
    iThreads_ = 1;//minimum threads if check fails for whatever reason
  }

  //initialize threadpool
  pvThreadPool_ = new std::vector<std::thread>;//has to be dynamically allocated due to not knowing thread count at run time
  pvThreadPool_->reserve(iThreads_);

  vJobProfiler_.reserve(iThreads_);
  for (int i = 0; i < iThreads_; ++i) {
    vJobProfiler_.emplace_back();
  }
}

void JobQueueManager::startThreads(std::vector<Job> vThreadSpecificJobs, std::vector<Job> vDistributedJobs)
{
  ///assign thread specific jobs
  for (int index = 0; index < vThreadSpecificJobs.size(); ++index) {
    jobQueue_.setThreadSpecificJob(vThreadSpecificJobs[index], index);
  }

  //assign distributable jobs to threads that aren't associated with specific ones
  int i = vThreadSpecificJobs.size();
  while (vDistributedJobs.size() > 0) {
    addJob(vDistributedJobs.back(), i);
    vDistributedJobs.pop_back();
    ++i;
    if (i >= iThreads_) {
      i = vThreadSpecificJobs.size();
    }
  }

  //initialize threads based on jobqueue status
  for (int i = 0; i < iThreads_-1; ++i) {
    pvThreadPool_->emplace_back(&JobQueueManager::workerThread, this);
  }
  workerThread();//last thread is main thread
}

JobQueueManager::~JobQueueManager()
{
  for (int i = 0; i < iThreads_ - 1; ++i) {//main thread doesn't join
    (*pvThreadPool_)[i].join();
    std::cout << "Thread " << i << " joined!" << std::endl;
  }
  std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - time_;
  std::cout << "Total Time: " << diff.count() << "s" << std::endl;
  delete pvThreadPool_;

  JobProfiler jobProfiler;
  for (int i = 0; i < iThreads_; ++i) {
    jobProfiler = jobProfiler + vJobProfiler_[i];
  }
  std::cout << jobProfiler;
}

void JobQueueManager::initializeThread(JobOptions& options)
{
  std::unique_lock<std::mutex> lock(mutWorkers_);
  options.myThreadId_ = workers_;
  ++workers_;

  options.bProfile_ = true;

  if (jobQueue_.hasThreadSpecificJob(options.myThreadId_)) {
    options.bThreadSpecificJob_ = true;
  }
}


void JobQueueManager::workerThread()
{
  JobOptions myJobOptions;
  initializeThread(myJobOptions);

  while ( true ) {
    mutaWaitForJob_.wait(myJobOptions.bThreadSpecificJob_);
    Job job = jobQueue_.getJob(myJobOptions.myThreadId_);
    try { doWork(job, myJobOptions); }
    catch (KillThreadException exception) { return; }//only killThread job should throw this exception
  }
}

void JobQueueManager::doWork(Job& job, JobOptions& options)
{
  std::chrono::steady_clock::time_point timeStart;
  if (options.bProfile_) {
    timeStart = std::chrono::high_resolution_clock::now();
  }
  job.doWork(options);
  if (options.bProfile_) {
    std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - timeStart;
    vJobProfiler_[options.myThreadId_].addTime(job.getName(), diff.count());
  }
}




void JobQueueManager::killThreads()
{
  std::cout << "starting killThreads" << std::endl;
  std::function<void(JobOptions)> func = std::bind(&JobQueueManager::killThread, this, std::placeholders::_1);
  for (int i = 0; i < jobQueue_.size(); ++i) {
    jobQueue_.setThreadSpecificJob(func, i);
    mutaWaitForJob_.notify_one();
    std::cout << "adding kill thread job to thread index: " << i << std::endl;
  }
}

void JobQueueManager::killThread(JobOptions options)
{
  KillThreadException exception;
  throw exception;
}

void JobQueueManager::addJob(Job job, int index)
{
  jobQueue_.addJob(job, index);
  mutaWaitForJob_.notify_one();
}




void JobQueueManager::addJob(void function(JobOptions), int index, std::string name)
{
  if (index < jobQueue_.size()) {
    std::function<void(JobOptions)> func = function;
    jobQueue_.addJob(func, index, name);
    mutaWaitForJob_.notify_one();
  }
}