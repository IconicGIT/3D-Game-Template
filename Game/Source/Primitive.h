
#pragma once
#include "glmath.h"
#include "Color.h"
#include "MathGeoLib/Geometry/GeometryAll.h"


enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:
	Primitive();

	void Update();
	virtual void	Render() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;


	Color color;
	mat4x4 transform;
	bool axis,wire;
	float lineWidth;

private:


	//TODO 4: Add a PhysBody to the primitive

protected:
	virtual void InnerRender() const;
	PrimitiveTypes type;
};

// ============================================
class Prim_Cube : public Primitive
{
public :
	Prim_Cube();
	Prim_Cube(float sizeX, float sizeY, float sizeZ);

	void SetSize(float sizeX, float sizeY, float sizeZ);
protected:
	void InnerRender() const;
public:
	vec3 size;
};

// ============================================
class Prim_Sphere : public Primitive
{
public:
	Prim_Sphere(float radius = 1.f, float mass = 1.f);

	void SetRadius(float radius, float mass = 1.f);
	void SetColumns(int n);
	void SetRows(int n);
protected:
	void InnerRender() const;
public:
	float slices, stacks;
	float radius;
};

// ============================================
class Prim_Cylinder : public Primitive
{
public:
	Prim_Cylinder();
	Prim_Cylinder(float radius, float height, float columns);

	void SetRadius(float radius);
	void SetHeight(float height);
	void SetColumns(float columns);

protected:
	void InnerRender() const;
public:
	float radius;
	float height;
	float columns;
};

// ============================================
class Prim_Line : public Primitive
{
public:
	Prim_Line();
	Prim_Line(float x, float y, float z);

	void SetOrigin(float x, float y, float z);
	void SetDestination(float x, float y, float z);
protected:
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Prim_Plane : public Primitive
{
public:
	Prim_Plane();
	Prim_Plane(float x, float y, float z, float d);

	void SetNormal(float x, float y, float z);
	void SetExtension(float e);

	Plane plane;
protected:
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
	float extension;
};