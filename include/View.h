#ifndef __Z64VIEW_H__
#define __Z64VIEW_H__
#include <Global.h>
#include <HermosauhuLib.h>
#include <Vector.h>
#include <Matrix.h>
#include <Input.h>

typedef struct {
	Vec3f eye;
	Vec3f at;
	s16   roll;
} Camera;

typedef struct {
	u8 smoothZoom : 1;
} CamSettings;

typedef struct {
	Vec3f vel;
	f32   speed;
} CameraFlyMode;

typedef struct {
	f32     fovy;
	f32     fovyTarget;
	f32     near;
	f32     far;
	f32     scale;
	f32     aspect;
	MtxF    viewMtx;
	MtxF    projMtx;
	CamSettings settings;
	Camera* currentCamera;
	Camera  camera[4];
	bool    cameraControl;
	bool    setCamMove;
	Vec2s   projectDim;
	CameraFlyMode flyMode;
} ViewContext;

void View_Camera_FlyMode(ViewContext* viewCtx, InputContext* inputCtx);
void View_Camera_OrbitMode(ViewContext* viewCtx, InputContext* inputCtx);

void View_Init(ViewContext* view, InputContext* input);
void View_Update(ViewContext* viewCtx, InputContext* inputCtx);

void View_SetProjectionDimensions(ViewContext* viewCtx, Vec2s* dim);

#endif