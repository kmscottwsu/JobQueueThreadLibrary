#ifndef JOB_PROFILER_H
#define JOB_PROFILER_H

#include "StdLibraryDependencies.h"

class JobProfiler
{
public:
  JobProfiler()
  {

  };

  ~JobProfiler()
  {
  };

  int size() const {
    return map_.size();
  };

  void addTime(std::string name, double count) {
    typedef std::map<std::string, double>::iterator JobProfilerIt;
    JobProfilerIt it = map_.find(name);
    if (it != map_.end()) {
      it->second += count;
    }
    else {
      map_.emplace(name, count);
    }
  };

  JobProfiler operator+(const JobProfiler& map2) {
    for (auto  it = map2.map_.begin(); it != map2.map_.end(); ++it) {
      addTime(it->first, it->second);
    }
    return *this;
  };

  friend std::ostream& operator<<(std::ostream& os, const JobProfiler& jobProfiler);

private:
  std::map<std::string, double> map_;

};

#endif