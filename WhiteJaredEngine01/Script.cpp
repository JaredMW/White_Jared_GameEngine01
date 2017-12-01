// Jared White
// May 16, 2017
// WhiteJaredEngine01 Script.h
#include "Script.h"
Script::Script()
{
}
Script::~Script()
{
}

// Get the Transform that this Script is attached to
Transform* Script::getTransform()
{
	return transform;
}