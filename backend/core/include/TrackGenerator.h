#pragma once
#include "Piece.h"
#include <json/json.h>
#include "ReadJson.h"
#include "Part.h"
#include "Obb.h"
#include "Vec2D.h"

/**
 * Generates tracks for a given set of pieces.
*/


/**
 * Connects pieces together to form a track. Returns the first piece of the track
*/
Piece generateTrack(const Json::Value& selection);