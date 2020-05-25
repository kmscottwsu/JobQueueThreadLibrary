#include "JobProfiler.h"

std::ostream& operator<<(std::ostream& os, const JobProfiler& jobProfiler)
{
  for (auto it = jobProfiler.map_.begin(); it != jobProfiler.map_.end(); ++it) {
    os << "function: " << it->first << "\t time: " << it->second << " seconds." << std::endl;;
  }
  return os;
};