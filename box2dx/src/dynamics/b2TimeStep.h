#ifndef B2_TIME_STEP_H
#define B2_TIME_STEP_H

#include "../common/b2Math.h"

/// Profiling data. Times are in milliseconds.
struct b2Profile
{
	float32 step;
	float32 collide;
	float32 solve;
	float32 solveInit;
	float32 solveVelocity;
	float32 solvePosition;
	float32 broadphase;
	float32 solveTOI;
};

/// This is an internal structure.
struct b2TimeStep
{
	float32 dt;			// time step
	float32 inv_dt;		// inverse time step (0 if dt == 0).
	float32 dtRatio;	// dt * inv_dt0
	int32 velocityIterations;
	int32 positionIterations;
	bool warmStarting;
};

/// This is an internal structure.
struct b2Position
{
	b2Vec2 c;
	float32 a;
};

/// This is an internal structure.
struct b2Velocity
{
	b2Vec2 v;
	float32 w;
};

/// Solver Data
struct b2SolverData
{
	b2TimeStep step;
	b2Position* positions;
	b2Velocity* velocities;
};

#endif
