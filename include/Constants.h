#pragma once

// Window size
constexpr int SCREEN_WIDTH  = 480;
constexpr int SCREEN_HEIGHT = 720;

constexpr int GROUND = SCREEN_HEIGHT - 78;

constexpr int SCROLL_SPEED = 3;

// Bird settings
constexpr int BIRD_SIZE = 32;
constexpr int BIRD_JUMP_SPEED = 9;

// Physics
constexpr float GRAVITY_MULTIPLIER = 2.5f;

// Pipe settings
constexpr int PIPE_WIDTH = 80;
constexpr int PIPE_HEIGHT = 500;
constexpr int PIPE_GAP = 150;
constexpr int PIPE_MIN_MARGIN = 60;  // Threshold