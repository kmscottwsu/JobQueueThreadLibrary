#include "Renderer.h"


Renderer::Renderer(JobQueueManager* pJobQueueManager) :
pJobQueueManager_(pJobQueueManager)
{
  window_ = NULL;
  //pJobQueueManager_->addJob(&Renderer::runThread, this);
}

Renderer::~Renderer()
{

}

void Renderer::runThread(const JobOptions)
{
  //SDL_Init(SDL_INIT_VIDEO);

  //window_ = SDL_CreateWindow(
    //"An SDL2 window",
    //10, 25, 640, 480, SDL_WINDOW_OPENGL);

  //SDL_Delay(5000);
  //pJobQueueManager_->addJob(&JobQueueManager::killThreads, pJobQueueManager_);

  //SDL_DestroyWindow(window_);
  //SDL_Quit();
}