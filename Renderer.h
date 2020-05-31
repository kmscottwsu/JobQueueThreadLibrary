#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"

#include "ManagerBaseClass.h"


class Renderer : ManagerBaseClass
{
public:
  Renderer(JobQueueManager* pJobQueueManager);
  ~Renderer();

  void run(const JobOptions);//SDL requires renderer to always be run by same thread

private:
  JobQueueManager* pJobQueueManager_;
  SDL_Window* window_;
  SDL_Surface* screenSurface_;


  int screenWidth_;
  int screenHeight_;

};

#endif