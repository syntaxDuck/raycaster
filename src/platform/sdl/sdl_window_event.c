#include "window_event.h"
#include <SDL.h>

bool poll_window_event(Window *win, WindowEvent *event_out)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_WINDOWEVENT && event.window.windowID == SDL_GetWindowID((SDL_Window *)win))
    {
      event_out->data1 = event.window.data1;
      event_out->data2 = event.window.data2;

      switch (event.window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
        event_out->type = WINDOW_EVENT_RESIZED;
        return true;

      case SDL_WINDOWEVENT_CLOSE:
        event_out->type = WINDOW_EVENT_CLOSE;
        return true;

      case SDL_WINDOWEVENT_SHOWN:
        event_out->type = WINDOW_EVENT_SHOWN;
        return true;
      case SDL_WINDOWEVENT_HIDDEN:
        event_out->type = WINDOW_EVENT_HIDDEN;
        return true;
      case SDL_WINDOWEVENT_FOCUS_GAINED:
        event_out->type = WINDOW_EVENT_FOCUS_GAINED;
        return true;
      case SDL_WINDOWEVENT_FOCUS_LOST:
        event_out->type = WINDOW_EVENT_FOCUS_LOST;
        return true;
      case SDL_WINDOWEVENT_ENTER:
        event_out->type = WINDOW_EVENT_ENTER;
        return true;
      case SDL_WINDOWEVENT_LEAVE:
        event_out->type = WINDOW_EVENT_LEAVE;
        return true;
      case SDL_WINDOWEVENT_MINIMIZED:
        event_out->type = WINDOW_EVENT_MINIMIZED;
        return true;
      case SDL_WINDOWEVENT_MAXIMIZED:
        event_out->type = WINDOW_EVENT_MAXIMIZED;
        return true;
      case SDL_WINDOWEVENT_RESTORED:
        event_out->type = WINDOW_EVENT_RESTORED;
        return true;
      default:
        break;
      }
    }
  }
  event_out->type = WINDOW_EVENT_NONE;
  return false;
}