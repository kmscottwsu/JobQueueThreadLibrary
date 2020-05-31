#include "Renderer.h"


Renderer::Renderer(JobQueueManager* pJobQueueManager) :
pJobQueueManager_(pJobQueueManager)
{
  window_ = NULL;
  screenSurface_ = NULL;

  screenWidth_ = 640;
  screenHeight_ = 480;


  //pJobQueueManager_->addJob(&Renderer::runThread, this, 0, "renderer");
}

Renderer::~Renderer()
{

}

void Renderer::run(JobOptions options)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError();
  }
  else {
    window_ = SDL_CreateWindow("SDL Tutorial",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      screenWidth_, screenHeight_, SDL_WINDOW_SHOWN);
    if (window_ == NULL) {
      std::cout << "Window could not be created! SDL_Error: " << SDL_GetError();
    }
    else {
      screenSurface_ = SDL_GetWindowSurface(window_);
      SDL_FillRect(screenSurface_, NULL, SDL_MapRGB(screenSurface_->format, 0, 0xFF, 0));

      SDL_UpdateWindowSurface(window_);

      SDL_Delay(10000);//can't accept inputs during delay

      SDL_DestroyWindow(window_);
      SDL_Quit();
    }
  }
}