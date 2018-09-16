#pragma once

#include "View.h"

struct statusview
{
	View*(*init)(void);	
};
extern const struct statusview StatusView;