#pragma once

/// Makes $() console out the passed values to them.
/// Comment this macro to disable $() debug prints.
/// To see more detailed information, see print.h++.
#define REMIXER_$_PRINTS_ON

/// Makes Remixer show an imgui debug audio visualiser.
/// Comment this macro to prevent showing the visualiser.
#define REMIXER_VISUAL_DEBUGGER_ON

/// Makes Runtime::interrupt exit the application.
/// Comment this macro to prevent Remixer exit on interruption.
#define REMIXER_EXIT_ON_INTERRUPTION_ON
