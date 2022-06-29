#include <algorithm>
#include <iostream>
#include "fm77av.h"
#include "fm77avdef.h"
#include "outside_world.h"



bool FM77AV::KeyIRQFlagSet(void) const
{
	if(CPU_SUB==state.keyboardIRQHandler)
	{
		return 0!=(state.sub.firqSource&SystemState::SUB_FIRQ_SOURCE_KEY);
	}
	else
	{
		return 0!=(state.main.irqSource&SystemState::MAIN_IRQ_SOURCE_KEY);
	}
}
void FM77AV::SetKeyIRQFlag(void)
{
	if(CPU_SUB==state.keyboardIRQHandler)
	{
		state.sub.firqSource|=SystemState::SUB_FIRQ_SOURCE_KEY;
	}
	else
	{
		state.main.irqSource|=SystemState::MAIN_IRQ_SOURCE_KEY;
	}
}
void FM77AV::ClearKeyIRQFlag(void)
{
	state.sub.firqSource&=~SystemState::SUB_FIRQ_SOURCE_KEY;
	state.main.irqSource&=~SystemState::MAIN_IRQ_SOURCE_KEY;
}
void FM77AV::SetBreakKeyFIRQFlag(void)
{
	state.main.firqSource|=SystemState::MAIN_FIRQ_SOURCE_BREAK_KEY;
}
void FM77AV::ClearBreakKeyFIRQFlag(void)
{
	state.main.firqSource&=~SystemState::MAIN_FIRQ_SOURCE_BREAK_KEY;
}
