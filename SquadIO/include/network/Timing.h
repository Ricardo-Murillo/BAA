#ifndef TIMING_H_INCLUDED
#define TIMING_H_INCLUDED
#include <time.h>
class Timing
{
public:

	Timing();

	SQUADIO_API bool Update();

	SQUADIO_API float GetDeltaTime() const { return mDeltaTime; }

	double GetTime() const;

	float GetTimef() const{
		return static_cast< float >(GetTime());
	}

	float GetFrameStartTime() const { return mFrameStartTimef; }

	SQUADIO_API static Timing sInstance;
	/// Begins the gameclock, called when game begins
	SQUADIO_API void SetCountdownStart();
	/// Starts 
	SQUADIO_API void StartAttackCooldown();
	/// Returns the number of seconds left on the game clock, used for displaying countdown timer
	SQUADIO_API int GetTimeRemainingS();
	/// Takes the time remaining in seconds and returns the minutes to be displayed
	SQUADIO_API string GetMinutesLeftAsString(int timeRemainingInSec);
	/// Takes the time remaining in seconds and returns the seconds to be displayed alongside minutes
	SQUADIO_API string GetSecondsLeftAsString(int timeRemainingInSec);
	/// Returns true if player's attack cooldown has finished
	SQUADIO_API bool AttackCooldownEnded();
	/// Returns true if minions should be spawned
	SQUADIO_API bool SpawnMinions();
private:
	float		mDeltaTime;
	uint64_t	mDeltaTick;

	int			minionCounter;
	int			gameLengthInSeconds;
	time_t		startTimeInSeconds;
	time_t		attackCooldown;
	double		mLastFrameStartTime;
	float		mFrameStartTimef;
	double		mPerfCountDuration;

};
#endif