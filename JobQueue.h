#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include "StdLibraryDependencies.h"

#include "Job.h"


class SubQueue
{
public:
  SubQueue(int i);
  ~SubQueue();

  void addJob(std::function<void(JobOptions)> func, std::string name = "UnnamedJob");
  void addJob(Job& job);
  bool findJob(Job& job, int& index);
  void setThreadSpecificJob(Job* pJob);
  bool hasThreadSpecificJob();

private:

  std::queue<Job> jobQueue_;
  std::atomic_flag bQueueInUse_;//only let one thread get jobs at a time
  std::atomic<int> iJobCount_;

  std::atomic<Job*> pThreadSpecificJob_;
  int iThreadId_;
};


class JobQueue
{
public:
  JobQueue(int& numThreads);
  JobQueue();
  ~JobQueue();

  void addJob(std::function<void(JobOptions)> func, int& index, std::string name = "UnnamedJob");
  void addJob(Job& job, int& index);
  Job getJob(int& index);
  void setThreadSpecificJob(Job* job, int& index);
  void resize(int& i);//this doesn't work
  int size();
  bool hasThreadSpecificJob(int& i);

private:
  std::vector<SubQueue*> vSubQueues_;
  int numThreads_;
};

#endif