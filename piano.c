#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define NUM_NOTES 13

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *keyTextures[NUM_NOTES];
SDL_Rect keyRects[NUM_NOTES];
bool keyActive[NUM_NOTES] = {false};

void loadSounds() {
    const char *notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2"};
    
    for (int i = 0; i < NUM_NOTES; ++i) {
        char filename[20];
        snprintf(filename, sizeof(filename), "sounds/%s.wav", notes[i]);
        keyTextures[i] = IMG_LoadTexture(renderer, filename);
        if (!keyTextures[i]) {
            fprintf(stderr, "Error loading texture for note %s\n", notes[i]);
            exit(EXIT_FAILURE);
        }

        keyRects[i].x = i * 60;
        keyRects[i].y = 0;
        keyRects[i].w = 50;
        keyRects[i].h = 100;
    }
}

void playKey(int index) {
    Mix_PlayChannel(-1, keySounds[index], 0);
    keyActive[index] = true;
}

void handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    exit(EXIT_SUCCESS);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                for (int i = 0; i < NUM_NOTES; ++i) {
                    if (event.button.x >= keyRects[i].x &&
                        event.button.x < keyRects[i].x + keyRects[i].w &&
                        event.button.y >= keyRects[i].y &&
                        event.button.y < keyRects[i].y + keyRects[i].h) {
                        playKey(i);
                    }
                }
                break;
        }
    }
}

void update() {
    for (int i = 0; i < NUM_NOTES; ++i) {
        keyActive[i] = false;
    }
}

void render() {
    SDL_RenderClear(renderer);
    for (int i = 0; i < NUM_NOTES; ++i) {
        SDL_RenderCopy(renderer, keyTextures[i], NULL, &keyRects[i]);
        if (keyActive[i]) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, &keyRects[i]);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
    }
    SDL_RenderPresent(renderer);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Virtual Piano", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 200, 0);
    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer initialization failed: %s\n", Mix_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    loadSounds();

    while (true) {
        handleEvents();
        update();
        render();
        SDL_Delay(10);  // Add a small delay to avoid high CPU usage
    }

    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
