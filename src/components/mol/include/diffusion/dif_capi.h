#pragma once

typedef struct Diffusion Diffusion;
typedef struct Coordinate {
    double x;
    double y;
    double z;
    double t;
} Coordinate;

Diffusion *make_diffusion();

void set_json(Diffusion* d, const char* json_str);
void get_json(Diffusion* d, char *buf);

void init(Diffusion *d);
void run(Diffusion *d);

void init_coord_io(Diffusion *d);
void init_collision_io(Diffusion *d);

bool next_coordinate(Diffusion *d, Coordinate *c);
bool next_collision(Diffusion *d, double *c);

void destroy_diffusion(Diffusion *d);
