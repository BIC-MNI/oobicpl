#ifndef __BICLABELVOLUME__
#define __BICLABELVOLUME__

extern "C" {
#include "bicpl.h"
#include "volume_io.h"
}

#include "bicVolume.h"
#include <iostream.h>

class bicLabelVolume : public bicVolume {
