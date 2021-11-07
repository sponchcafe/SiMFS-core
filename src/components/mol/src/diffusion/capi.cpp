#include <cstring>
#include "diffusion/dif_capi.h"
#include "diffusion/component.hpp"

using json = nlohmann::json;

struct Diffusion {
    std::unique_ptr<sim::comp::Diffusion> dif_;
    std::unique_ptr<sim::io::BufferInput<sim::Coordinate>> coordinates;
    std::unique_ptr<sim::io::BufferInput<sim::realtime_t>> collisions;
};

Diffusion *make_diffusion(){
    Diffusion *d = new Diffusion;
    d->dif_ = std::make_unique<sim::comp::Diffusion>();
    return d; }

void set_json(Diffusion *d, const char *json_str) {
    auto cfg = json::parse(json_str);
    d->dif_->set_json(cfg);
}

void get_json(Diffusion *d, char *buf) {
    auto cfg = d->dif_->get_json();
    std::string s = cfg.dump();
    strcpy(buf, s.c_str());
}

void init(Diffusion *d) {
    d->dif_->init();
}

void init_coord_io(Diffusion *d){
    d->coordinates = std::make_unique<sim::io::BufferInput<sim::Coordinate>>(
        d->dif_->get_json()["coordinate_output"]
    );
}

void init_collision_io(Diffusion *d){
    d->collisions = std::make_unique<sim::io::BufferInput<sim::realtime_t>>(
        d->dif_->get_json()["collision_output"]
    );
}

void run(Diffusion *d) {
    d->dif_->run();
    d->dif_ = nullptr; // delete diffusion component to flus outputs
}

void destroy_diffusion(Diffusion *d){
    delete d;
}

bool next_coordinate(Diffusion *d, Coordinate *c) {
    sim::Coordinate c_{0.0, 0.0, 0.0, 0.0};
    bool ret = d->coordinates->get(c_);
    c->x = c_.x;
    c->y = c_.y;
    c->z = c_.z;
    c->t = c_.t;
    return ret;

}

bool next_collision(Diffusion *d, double *c){
    return d->collisions->get(*c);
}
