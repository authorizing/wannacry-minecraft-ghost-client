#include "modules.h"

//autoclicker

bool aclicker::enabled = false;

int aclicker::key = 0;

float aclicker::cps_min = 10.f;
float aclicker::cps_max = 10.f;

float aclicker::jitter_x = 0.f;
float aclicker::jitter_y = 0.f;

int aclicker::blockhit_delay = 500;

int aclicker::breakblocks_mode = 0;
/* 0 = off, 1 = native, 2 = rmb lock, 3 = dynamic */

//flags


//aim assist

bool aim::enabled = false;

float aim::max_angle = 180.f;
float aim::aim_speed = 10.0f;
float aim::aim_distance = 6.f;

int aim::method = 0;
int aim::hitbox = 0;

//flags

bool aim::on_click = true;
bool aim::on_move = false;
bool aim::vertical = false;
bool aim::antibot = false;

//reach

float reach::distance_min = 3.f;
float reach::distance_max = 3.f;

float reach::chance = 100.f;



//velocity

float velocity::horizontal = 1.;
float velocity::vertical = 1.;



