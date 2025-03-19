/*
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/*
#include "JSL/JoyShockLibrary.h"
*/

#define SDL_WINDOW_WIDTH  640
#define SDL_WINDOW_HEIGHT 480
#define SDL_RENDER_VSYNC 1
bool isfullscreen;
bool changefullscreen = false;

/*
#define SDL_WINDOW_WIDTH           (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_WIDTH)
#define SDL_WINDOW_HEIGHT          (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_HEIGHT)

#define SNAKE_GAME_WIDTH  24U
#define SNAKE_GAME_HEIGHT 18U
#define SNAKE_MATRIX_SIZE (SNAKE_GAME_WIDTH * SNAKE_GAME_HEIGHT)
*/

static SDL_Gamepad *gamepad = NULL;

/*
#define SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS "SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS"
#define SDL_HINT_TRACKPAD_IS_TOUCH_ONLY "SDL_TRACKPAD_IS_TOUCH_ONLY"
#define SDL_HINT_GAMECONTROLLER_SENSOR_FUSION "SDL_GAMECONTROLLER_SENSOR_FUSION"
*/
/*
*/

typedef struct
{
    SDL_Window *window;
    SDL_WindowFlags windowFlags;
    SDL_Renderer *renderer;
} AppState;

static SDL_AppResult handle_key_event_(SDL_Scancode key_code)
{
/*
    printf("Keyboard: %s\n", key_code);
*/
    switch (key_code) {
    /* Quit. */
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;
    /* Fullscreen */
    case SDL_SCANCODE_F:
        changefullscreen = !changefullscreen;
        break;
    case SDL_SCANCODE_RIGHT:
        break;
    case SDL_SCANCODE_UP:
        break;
    case SDL_SCANCODE_LEFT:
        break;
    case SDL_SCANCODE_DOWN:
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult handle_hat_event_(Uint8 hat) 
{
    printf("D-Pad: %d\n", hat);
    switch (hat) {
    case SDL_HAT_RIGHT:
        break;
    case SDL_HAT_UP:
        break;
    case SDL_HAT_LEFT:
        break;
    case SDL_HAT_DOWN:
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult handle_btn_event_(SDL_Gamepad *gamepad, Uint8 button) 
{
    printf("Button: ");
    char *debugBtn = (char*)malloc(17 * sizeof(char));
    switch (button) {
    case SDL_GAMEPAD_BUTTON_GUIDE:
        return SDL_APP_SUCCESS;
    case SDL_GAMEPAD_BUTTON_SOUTH:
        strcpy(debugBtn, "South");
        break;
    case SDL_GAMEPAD_BUTTON_EAST:
        strcpy(debugBtn, "East");
        break;
    case SDL_GAMEPAD_BUTTON_WEST:
        strcpy(debugBtn, "West");
        break;
/*
        case SDL_GAMEPAD_BUTTON_MISC1:
        case SDL_GAMEPAD_BUTTON_TOUCHPAD:
*/
    case SDL_GAMEPAD_BUTTON_BACK:
        char *debugFull = (char*)malloc(20 * sizeof(char));
        strcpy(debugFull, isfullscreen ? "Back from Fullscreen" : "Back from Windowed");
        strcpy(debugBtn, debugFull);
        free(debugFull);
        changefullscreen = !changefullscreen;
        break;
    default:
        strcpy(debugBtn, "....");
        break;
    }
    printf("%s\n", debugBtn);
    free(debugBtn);
    return SDL_APP_CONTINUE;
}
static SDL_AppResult handle_pad_event_(SDL_Gamepad *gamepad, float pad_x, float pad_y) 
{
/*
        SDL_Log("%f : ", pad_x);    
        SDL_Log("%f\n", pad_y); 
*/
    return SDL_APP_CONTINUE;
}

static SDL_AppResult handle_sense_event_(SDL_Gamepad *gamepad, Sint32 sensortype)
{
    float data[3];
    if(!SDL_GetGamepadSensorData(gamepad, sensortype, data, 3)) {
        SDL_Log("Couldn't get sensor data: %s\n", SDL_GetError());
                return SDL_APP_CONTINUE;
    }
    switch(sensortype) {
        case SDL_SENSOR_GYRO:
            printf("Gyro: ");
            break;
        case SDL_SENSOR_ACCEL:
            printf("Accel: ");
            break;
        default:
            break;
    }
    for(int i=0; i < 3; i++)
    {
        printf("%f  :  ", data[i]);
    }
    SDL_Log("\n");
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *as = (AppState *)appstate;
    if(changefullscreen) {
      if (!SDL_SetWindowFullscreen(as->window, !(isfullscreen))) {
        SDL_Log("Couldn't change: %s", SDL_GetError());
      }
      isfullscreen = !isfullscreen;
      printf("%s\n", isfullscreen ? "Now in full screen" : "Now in windowed");
      changefullscreen = !changefullscreen;
    }
    int w_height, w_width;
    SDL_Rect displayBounds;
    SDL_GetWindowSize(as->window, &w_width, &w_height);
    if(isfullscreen) {
        if(!SDL_GetDisplayBounds(SDL_GetDisplayForWindow(as->window), &displayBounds)) {
            SDL_Log("Couldn't get display bounds: %s", SDL_GetError());
        }
    }

    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(as->renderer);

    const Uint64 now = SDL_GetTicks(); //milliseconds
    const double dnow = ((double)now / 1000.0); //seconds

    // run game logic if we're at or past the time to run it.
    // if we're _really_ behind the time to run it, run it
    // several times.
    const float red = (float) (0.5 + 0.5 * SDL_sin(dnow));
    const float green = (float) (0.5 + 0.5 * SDL_sin(dnow + SDL_PI_D * 2 / 3));
    const float blue = (float) (0.5 + 0.5 * SDL_sin(dnow + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(as->renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

    SDL_FRect rects[1];
    const float scale = (float)(((int) dnow - 500) / 500.0f);
    rects[0].h = w_height / 8;
    rects[0].w = w_width / 4;
    rects[0].x = (w_width - rects[0].w) / 2;
    rects[0].y = (w_height - rects[0].h) / 2;
    if(isfullscreen) {
        rects[0].h = displayBounds.h / 8;
        rects[0].w = displayBounds.w / 4;
        rects[0].x = (displayBounds.w - rects[0].w) / 2;
        rects[0].y = (displayBounds.h - rects[0].h) / 2;
    }

    SDL_RenderFillRects(as->renderer, rects, SDL_arraysize(rects));

    SDL_RenderPresent(as->renderer);
    return SDL_APP_CONTINUE;
}

static const struct
{
    const char *key;
    const char *value;
} extended_metadata[] =
{
    { SDL_PROP_APP_METADATA_URL_STRING, "steamlink-sdk/examples/screen" },
    { SDL_PROP_APP_METADATA_CREATOR_STRING, "DamianS-eng" },
    { SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Placed in the public domain" },
    { SDL_PROP_APP_METADATA_TYPE_STRING, "test" }
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    size_t i;
    if (!SDL_SetAppMetadata("Gamepad Screen", "1.0", "com.example.Screen")) {
        return SDL_APP_FAILURE;
    }
    for (i = 0; i < SDL_arraysize(extended_metadata); i++) {
        if (!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value)) {
            return SDL_APP_FAILURE;
        }
    }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!as) {
        return SDL_APP_FAILURE;
    }
    *appstate = as;
    as->windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN);
    if(as->windowFlags & SDL_WINDOW_FULLSCREEN) { 
      isfullscreen = true;
      printf("Set to fullscreen");
    }
    if (!SDL_CreateWindowAndRenderer("examples/screen", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, as->windowFlags, &as->window, &as->renderer)) {
        return SDL_APP_FAILURE;
    }
    SDL_Log("Initialize Screen");
		if(!SDL_RENDER_VSYNC | !SDL_SetRenderVSync( as->renderer, 1 ) )
    {
        SDL_Log( "Could not enable VSync! SDL error: %s\n", SDL_GetError() );
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_GAMEPAD_ADDED:
    if (gamepad == NULL) {
      gamepad = SDL_OpenGamepad(event->gdevice.which);
      if (!gamepad) {
        SDL_Log("Failed to open Gamepad ID %u: %s\n", (unsigned int) event->gdevice.which, SDL_GetError());
      }
    char *gamepad_mapping = SDL_GetGamepadMapping(gamepad);
    int numTouches = SDL_GetNumGamepadTouchpads(gamepad);
    if(numTouches > 0) {
      SDL_Log("Touchpads: %d", numTouches);
    }
    Uint64 gamepad_steam = SDL_GetGamepadSteamHandle(gamepad);
    SDL_Log("Controller mapping: %s\n", gamepad_mapping);
    if(gamepad_steam != 0) {
      SDL_Log("Steam Handle: %d\n", gamepad_steam);
    }
    if(SDL_GamepadHasSensor(gamepad, SDL_SENSOR_GYRO) & !SDL_EventEnabled(SDL_EVENT_GAMEPAD_SENSOR_UPDATE)) {
      SDL_Log("Gyro Event not active.");
    } else {
      SDL_SetGamepadSensorEnabled(gamepad, SDL_SENSOR_GYRO, true);
    }
/*
    SDL_Log("Connected Joystick %s", SDL_GetGamepadNameForID(gamepad_id));
    SDL_Log("GUID: %d", SDL_GetGamepadGUIDForID(gamepad_id));
*/
    }
    break;
  case SDL_EVENT_GAMEPAD_REMOVED:
    if (gamepad && (SDL_GetGamepadID(gamepad) == event->gdevice.which)) {
      SDL_CloseGamepad(gamepad);
      gamepad = NULL;
    }
    break;
  case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
    return handle_pad_event_(gamepad, event->gtouchpad.x, event->gtouchpad.y);
  case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
    return handle_sense_event_(gamepad, event->gsensor.sensor);
  case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
    break;
  case SDL_EVENT_GAMEPAD_AXIS_MOTION:
    break;
  case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    return handle_btn_event_(gamepad, event->gbutton.button);
  case SDL_EVENT_KEY_DOWN:
    return handle_key_event_(event->key.scancode);
  default:
    break;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  if (gamepad) {
    SDL_CloseGamepad(gamepad);
  }
  if (appstate != NULL) {
    AppState *as = (AppState *)appstate;
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
}
