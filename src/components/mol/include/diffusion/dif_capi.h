#pragma once

typedef struct Diffusion Diffusion;

Diffusion *make_diffusion();

void set_json(Diffusion* diff, const char* json);
void get_json(Diffusion* diff, char *buf);
void init(Diffusion *diff);
void run(Diffusion *diff);

void destroy_diffusion(Diffusion *d);
