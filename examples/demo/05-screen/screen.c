/*
 * Logic implementation of the Snake game. It is designed to efficiently
 * represent the state of the game in memory.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define STEP_RATE_IN_MILLISECONDS  125
#define SNAKE_BLOCK_SIZE_IN_PIXELS 24
#define SDL_WINDOW_WIDTH           (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_WIDTH)
#define SDL_WINDOW_HEIGHT          (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_HEIGHT)

#define SNAKE_GAME_WIDTH  24U
#define SNAKE_GAME_HEIGHT 18U
#define SNAKE_MATRIX_SIZE (SNAKE_GAME_WIDTH * SNAKE_GAME_HEIGHT)

static SDL_Joystick *joystick = NULL;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} AppState;

static SDL_AppResult handle_key_event_(SDL_Scancode key_code)
{
    switch (key_code) {
    /* Quit. */
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;
    /* Restart the game as if the program was launched. */
    case SDL_SCANCODE_R:
        break;
    /* Decide new direction of the snake. */
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

static SDL_AppResult handle_hat_event_(Uint8 hat) {
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

static SDL_AppResult handle_btn_event(Uint8 button) {
		SDL_Log("%d \n", button);
    switch (button) {
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *as = (AppState *)appstate;
    const Uint64 now = SDL_GetTicks();

    // run game logic if we're at or past the time to run it.
    // if we're _really_ behind the time to run it, run it
    // several times.

    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(as->renderer);
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
    if (!SDL_SetAppMetadata("Example Screen", "1.0", "com.example.Screen")) {
        return SDL_APP_FAILURE;
    }
    for (i = 0; i < SDL_arraysize(extended_metadata); i++) {
        if (!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value)) {
            return SDL_APP_FAILURE;
        }
    }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!as) {
        return SDL_APP_FAILURE;
    }
    *appstate = as;

    if (!SDL_CreateWindowAndRenderer("examples/screen", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, 0, &as->window, &as->renderer)) {
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_JOYSTICK_ADDED:
        if (joystick == NULL) {
            joystick = SDL_OpenJoystick(event->jdevice.which);
            if (!joystick) {
                SDL_Log("Failed to open joystick ID %u: %s", (unsigned int) event->jdevice.which, SDL_GetError());
            }
        }
        break;
    case SDL_EVENT_JOYSTICK_REMOVED:
        if (joystick && (SDL_GetJoystickID(joystick) == event->jdevice.which)) {
            SDL_CloseJoystick(joystick);
            joystick = NULL;
        }
        break;
    case SDL_EVENT_JOYSTICK_HAT_MOTION:
        break;
    case SDL_EVENT_KEY_DOWN:
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (joystick) {
        SDL_CloseJoystick(joystick);
    }
    if (appstate != NULL) {
        AppState *as = (AppState *)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
}
