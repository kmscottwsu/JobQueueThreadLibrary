#ifndef JOB_H
#define JOB_H

#include "JobOptions.h"

#include "StdLibraryDependencies.h"

class Job
{
private:
	std::function<void(JobOptions)> func_;
	bool bValid_;
	std::string name_;

public:
	Job(std::function<void(JobOptions)> func, std::string name = "UnnamedJob")
	{
		func_ = func;
		bValid_ = true;
		name_ = name;
	}

	template<class T>
	Job(void (T::* function)(JobOptions), T* p, std::string name = "UnnamedJob")
	{
		func_ = std::bind(function, p, std::placeholders::_1);
		bValid_ = true;
		name_ = name;
	}

	Job(const Job& job)
	{
		func_ = job.func_;
		bValid_ = job.bValid_;
		name_ = job.name_;
	}



	Job()
	{
		func_ = NULL;
		bValid_ = false;
	}

	~Job()
	{

	}

	void doWork(JobOptions& options)
	{
		if (isValid()) {
			func_(options);
		}
	}

	std::string getName()
	{
		return name_;
	}

	bool isValid()
	{
		return bValid_;
	}

};

#endif