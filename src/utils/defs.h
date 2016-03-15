#pragma once

#define from(WHO, WHAT) QObject::connect(WHO, WHAT,
#define to(WHO, WHERE) WHO, WHERE)
