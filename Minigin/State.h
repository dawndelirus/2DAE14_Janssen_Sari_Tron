#pragma once

class State
{
public:
	State() = default;
	virtual ~State() = default;

	State(const State& other) = delete;
	State(State&& other) = delete;
	State& operator=(const State& other) = delete;
	State& operator=(State&& other) = delete;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
};

