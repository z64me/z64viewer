#ifndef __Z64LIGHT_H__
#define __Z64LIGHT_H__
#include <HermosauhuLib.h>
#include <Vector.h>

typedef enum {
	LIGHT_POINT_NOGLOW,
	LIGHT_DIRECTIONAL,
	LIGHT_POINT_GLOW
} LightType;

typedef struct {
	s16  x;
	s16  y;
	s16  z;
	RGB8 color;
	u8   drawGlow;
	s16  radius;
} LightPoint;

typedef struct {
	s8   x;
	s8   y;
	s8   z;
	RGB8 color;
} LightDirectional;

typedef union {
	LightPoint point;
	LightDirectional dir;
} LightParams;

typedef struct {
	u8 type;
	LightParams params;
} LightInfo;

typedef struct {
	RGB8  ambient;
	Vec3c dirA;
	RGB8  colorA;
	Vec3c dirB;
	RGB8  colorB;
	RGB8  fogColor;
	s16   fogNear;
	s16   fogFar;
} EnvLight;

typedef struct {
	EnvLight* envLight;
	u8 lightListNum;
	u8 curLightId;
} LightContext;

struct Scene;
void Light_BindLights(struct Scene* scene);

#endif