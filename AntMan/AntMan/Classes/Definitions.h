#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#define DISPLAY_TIME_SPLASH_SCENE 2
#define TRANSITION_TIME 0.5

#define PIPE_GAP 10
#define UPPER_SCREEN_PIPE_THRESHOLD 0.5
#define LOWER_SCREEN_PIPE_THRESHOLD 0.1

#define PIPE_MOVEMENT_SPEED 0.007
#define PIPE_SPAWN_FREQUENCY 0.002

// Collision mask
#define ANTMAN_COLLISION_BITMASK    0x000001
#define OBSTACLE_COLLISION_BITMASK  0x000002
#define POINT_COLLISION_BITMASK     0x000003
#define GROUND_COLLISION_BITMASK    0x000004
#define WORLD_COLLISION_BITMASK     0x000005
#define FLY_COLLISION_BITMASK       0x000006
#define DIE_COLLISION_BITMASK       0x000007

// AntMan speed
#define ANTMAN_FALLING_SPEED 200
#define ANTMAN_FALLING_ACC -450
#define ANTMAN_FLYING_SPEED 400
#define ANTMAN_RUSHING_SPEED 500
// #define ANTMAN_FLY_DURATION 0.05
#define ANTMAN_FLY_WITH_FLY_SPEED 150


// grond speed
#define GROUND_SPAWN_FREQUENCY 0.003
#define GROUND_MOVEMENT_SPEED 0.005
#define GROUND_SPEED 3

// coin speed
#define SCORE_MOVEMENT_SPEED 0.007
#define SCORE_SPAWN_FREQUENCY 0.001

// fly
#define FLY_SPAWN_FREQUENCY 0.005

#endif
