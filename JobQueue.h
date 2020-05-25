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
  void setThreadSpecificJob(std::function<void(JobOptions)> func, std::string name = "UnnamedJob");
  void setThreadSpecificJob(Job& job);
  bool hasThreadSpecificJob();

private:
  std::queue<Job> queue_;
  std::mutex mut_;
  Job jobThreadSpecific_;
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
  void setThreadSpecificJob(std::function<void(JobOptions)> func, int& index);
  void setThreadSpecificJob(Job& job, int& index);
  void resize(int& i);//this doesn't work
  int size();
  bool hasThreadSpecificJob(int& i);

private:
  std::vector<SubQueue*> vSubQueues_;
  int numThreads_;
};

#endif