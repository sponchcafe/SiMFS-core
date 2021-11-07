#include <cstring>
#include "diffusion/dif_capi.h"
#include "diffusion/component.hpp"

using json = nlohmann::json;

struct Diffusion {
    std::unique_ptr<sim::comp::Diffusion> dif_;
};

Diffusion *make_diffusion(){
    Diffusion *d = new Diffusion;
    d->dif_ = std::make_unique<sim::comp::Diffusion>();
    return d;
}

void set_json(Diffusion *d, const char *json_str) {
    auto cfg = json::parse(json_str);
    d->dif_->set_json(cfg);
}

void get_json(Diffusion *d, char *buf) {
    auto cfg = d->dif_->get_json();
    std::string s = cfg.dump();
    strcpy(buf, s.c_str());
}

void init(Diffusion *diff) {
    diff->dif_->init();
}

void run(Diffusion *diff) {
    diff->dif_->run();
}

void destroy_diffusion(Diffusion *d){
    delete d;
}

