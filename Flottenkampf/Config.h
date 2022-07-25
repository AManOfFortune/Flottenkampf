#pragma once

extern int WORLD_SIZE;
extern int NUM_OF_TEAMS;
extern int MAX_SHIPS_PER_TEAM;

#define SPEED 1000

enum shipDirections {
	UP = '^',
	DOWN = 'v',
	LEFT = '>',
	RIGHT = '<'
};

enum shipActions {
	MOVE = 'd',
	AIM = 'a',
	HIT = 'h',
	MISS = 'm'
};