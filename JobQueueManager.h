#ifndef JOB_QUEUE_MANAGER_H
#define JOB_QUEUE_MANAGER_H

#include "StdLibraryDependencies.h"

#include "Mutaphore.h"
#include "KillThreadException.h"
#include "JobQueue.h"
#include "JobProfiler.h"

class JobQueueManager
{
public:
	//public interface
	JobQueueManager();
	~JobQueueManager();

	void startThreads(std::vector<Job> vThreadSpecificJobs, std::vector<Job> vDistributedJobs);

	void killThreads();

	template<class T>
	void addJob(void (T::* function)(JobOptions), T* p, int index, std::string name = "UnnamedJob")
	{
		if (index < jobQueue_.size()) {
			std::function<void(JobOptions)> func = std::bind(function, p, std::placeholders::_1);
			jobQueue_.addJob(func, index, name);
			mutaWaitForJob_.notify_one();
		}
	};

	void addJob(void function(JobOptions), int index, std::string name = "UnnamedJob");
	void addJob(Job job, int index);

private:

	//thread functions
	void waitForThreads();

	void createWorkerThread(int i);
	void workerThread(bool bHighPriority = false);
	void killThread(JobOptions options);
	void initializeThread(int& i, JobOptions& options);
	void doWork(Job& job, JobOptions& options);

	//member variables
	int iThreads_;
	std::vector<std::thread>* pvThreadPool_;
	std::vector<JobProfiler> vJobProfiler_;

	JobQueue jobQueue_;
	Mutaphore mutaWaitForJob_;

	int workers_;
	std::mutex mutWorkers_;

	std::chrono::steady_clock::time_point time_;

};

#endif