#ifndef JOB_OPTIONS_H
#define JOB_OPTIONS_H

class JobOptions
{
public:
	JobOptions(bool bLog, bool bProfile) {
		bLog_ = bLog;
		bProfile_ = bProfile;
		myThreadId_ = 0;
	};
	JobOptions() {
		//default constructor
		bLog_ = false;
		bProfile_ = false;
		myThreadId_ = 0;
	};
	~JobOptions() {};

	bool bLog_;
	bool bProfile_;

	int myThreadId_;
};

#endif