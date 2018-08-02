/* ==========================================================================
   >File: GameUtilities.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Functions use in the game, mostly revolving around Entities.
   ========================================================================== */

#pragma once

#include "GraphicsUtilities.h"



// Entity Type IDs
#define ENTITY_ID_NONE          0
#define ENTITY_ID_BORDER        100
#define ENTITY_ID_SHIP          101
#define ENTITY_ID_SHIP_BULLET   102
#define ENTITY_ID_ASTEROID      103
#define ENTITY_ID_SAUCER        104
#define ENTITY_ID_SAUCER_BULLET 105
#define ENTITY_ID_FLOWER        106

// drawProperties mask flags
#define DRAW_POINTS               (1 << 0)
#define DRAW_LINES                (1 << 1)
#define DRAW_TRIANGLES            (1 << 2)
#define DRAW_NORMALS              (1 << 3)
#define DRAW_TRIANGLE_FRAMES      (1 << 4)
#define DRAW_DISTANCE_SHADING_OFF (1 << 5)

struct Entity
{    
    Entity() : typeID(ENTITY_ID_NONE), collidable(true), mass(0), drawProperties(DRAW_TRIANGLES) {}

    // update
    // ====================================================================== //
    // Update this entitie's location, orientation, and shape by applying
    // its velocity and angular velcity.
    void update();

    // getShapeInWorldSpace
    // ====================================================================== //
    // Get the shape of this entity after applying the translation and
    // rotation required to get it into world space.
    Shape getShapeInWorldSpace() const;

    // updateWorldSpaceShape
    // ====================================================================== //
    // Updata the world space shape memeber variable with the current
    // location and orientation.
    void updateWorldSpaceShape();

    // This identifies what type of Entity this is durring collisions and
    // directs where to looks for this entity durrig deleation.
    unsigned typeID;

    // This variable's use varies from entity to Entity.
    // > ENTITY_ID_SHIP = how many collisions this ship can survive
    //                    (negative means its currently in an invincible state)
    // > ENTITY_ID_ASTEROID = how many times this asteroid can split
    // > ENTITY_ID_SAUCER = how many updates untill it fires a bullet
    int health;

    // Can this Entity collide with other Entities (This doesn't affect border
    // collisions.)
    bool collidable;

    // This is used at a faster initial test to see if Entities could be
    // colliding.
    float boundingRadius;

    // The frame holds the points, lines, and triangles that represent this
    // Entity.
    Frame frame;

    // The location of this Entity in world space. (default is 0, 0, 0)
    Point locationPoint;

    // How this entity is oriented. (default is facing the positve x direction
    // with positive y as the up direction and positive z to the right)
    Quaternion orientation;

    // The distance this Entity moves every time update() is called.
    Vector velocity;

    // How much is entity rotates every time update() is called.
    Quaternion angularVelocity;

    // Mass is used to calculate momentum transfer durring collisions.
    float mass;

    // These flags determine gow this Entity is drawn.
    // bit  0 = 1 if drawing points
    // bit  1 = 1 if drawing lines
    // bit  2 = 1 if drawing triangles
    // bit  3 = 1 if drawing normals
    // bit  4 = 1 if drawing only triangle frames (ignore if bit 2 == 0)
    // bit  5 = 1 if distance shading is turned off
    // bit  6 =
    // bit  7 =
    uint8_t drawProperties;

    // Shape generated in world space, ready to calcualte collision.
    Shape worldSpaceShape;
};


// alignPlayCamera
// ========================================================================== //
// Align a camera's centerOfAttention, cameraLocation, and upDirection so
// that it's looking from behind and slightly above a given entity in 3rd
// person.
// 
// @param
// * const Entity & entity, the entity being aligned to
// * Camera & camera, camera being aligned
// * float d, the distance the camera is from the center of the entity
// * float f, what fraction of the rotation toward the desired aligment should
//            the camera be moved
void alignPlayCamera(const Entity & entity, Camera & camera, float d, float f);


// alignZoomCamera
// ========================================================================== //
// Align a camera's centerOfAttention, cameraLocation, and upDirection so
// that it's looking from the center of the given entity out in a 1st person
// view.
// 
// @param
// * const Entity & entity, the entity being aligned to
// * Camera & camera, camera being aligned
// * float d, the distance the center of attention is from the center of the
//            entity
void alignZoomCamera(const Entity & entity, Camera & camera, float d, float f);


// initializeShipFrame
// ========================================================================== //
// Turn a given frame into a ship's frame.
// 
// @params
// * Frame & frame, frame being modified
// * Color color0, color of the tip and rear
// * Color color1, color of the top and bottom
// * Color color2, color of the wing tips
void initializeShipFrame(Frame & frame, Color color0, Color color1, Color color);


// initializeSaucerFrame
// ========================================================================== //
// Turn the given frame into a saucer's frame.
// 
// @params
// * Frame & frame, frame being modified
// * float r, saucer's radius (determins size in general)
// * Color color0, color of the top
// * Color color1, color of the bottom
void initializeSaucerFrame(Frame & frame, float r, Color color0, Color color1);


// initializeBulletFrame
// ========================================================================== //
// Turn the given frame into a builet's frame.
// 
// @parmas
// * Frame & frame, the frame being modified
// * Color color, color of the frame
void initializeBulletFrame(Frame & frame, Color color);


// initializeLongBulletFrame
// ========================================================================== //
// Turn the given frame into a builet's frame.
// 
// @parmas
// * Frame & frame, the frame being modified
// * Color color, color of the frame
void initializeLongBulletFrame(Frame & frame, Color color);


// initializeAsteroidFrame
// ========================================================================== //
// Turn a given frame into an asteroid frame. Parameters determine the radius
// and number of triangles used to form said asteroid.
//
// @params
// * Frame & frame, frame being modified
// * float r, aproximate radius of the asteroid
// * float n, number of times triangles get subdivided to make this
//            asteroid more spherical
// * float d, determens the depth of dents. ex: 2 would mean half the radius
//            and 4 would be a quarter of the radius
// * Color color, color of the frame
void initializeAsteroidFrame(Frame & frame, float r, float n, float d, Color color);


// initializeBorderFrame
// ========================================================================== //
// Turn a given frame into a border frame. It looks similar to the asteroid
// frame, but there are no dents and all the triangle normals face towards
// it's center.
// 
// @params
// * Frame & frame, frame being modified
// * float r, aproximate radius of the border
// * float n, number of times triangles get subdivided to make this it more
//            spherical
// * Color color, color of the frame
void initializeBorderFrame(Frame & frame, float r, float n, Color color);


// createShip
// ========================================================================== //
// Create a ship entity. (Health not set)
// 
// @params
// * Color color0 = COLOR_GREEN, color of the tip and rear
// * Color color1 = COLOR_BLUE, color of the top and bottom
// * Color color2 = COLOR_RED, color of the wing tips
// 
// @return
// pointer to said ship entity
Entity* createShip(Color color0 = COLOR_GREEN, Color color1 = COLOR_BLUE, Color color2 = COLOR_RED);


// createSaucer
// ========================================================================== //
// Create a saucer entity
// 
// @params
// * Color color0 = COLOR_YELLOW, color of the top
// * Color color1 = COLOR_TEAL, color of the bottom
// 
// @return
// pointer to said saucer entity
Entity * createSaucer(Color color0 = COLOR_YELLOW, Color color1 = COLOR_TEAL);


// createBullet
// ========================================================================== //
// Create a bullet entity. Doesn't set the typeID.
//
// @params
// * Color color = COLOR_MAGENTA, its color
// 
// @return
// pointer to said entity
Entity* createBullet(Color color = COLOR_MAGENTA);


// createLongBullet
// ========================================================================== //
// Create a bullet entity. Doesn't set the typeID.
//
// @params
// * Color color = COLOR_RED, its color
// 
// @return
// pointer to said entity
Entity* createLongBullet(Color color = COLOR_RED);


// createAsteroid
// ========================================================================== //
// Create an asteroid entity. The size will be determined by the given int.
// 
// 0 == small asteroid
// 1 == medium asteroid
// 2 == large asteroid
// 
// @params
// * unsigned size, asteroid size
// * Color color = COLOR_WHITE, its color
// 
// @ return
// pointer to the asteroid entity
Entity* createAsteroid(unsigned size, Color color = COLOR_WHITE);


// createBorder
// ========================================================================== //
// Create a border entity.
// 
// @params
// * float r, radius of the border
// * float n, number of times the triangles making up the border are
//            subdivided. (Makes it more sphereical.)
// * Color color = COLOR_GREEN, its color
// 
// @ return
// pointer to a border entity
Entity* createBorder(float r, float n, Color color = COLOR_GREEN);


// createTrail
// ========================================================================== //
// Create a trail entity. Each point marks its previous locations, with the
// first point (index 0) being the current location.
// 
// @params
// * unsigned segments, the number of segments in the trail
// * Color first, color of the first segment (near entity)
// * Color last, color of the last segment
// 
// @return
// pointer to a trail entity
Entity* createTrail(unsigned segments, Color first, Color last);


// createLader
// ========================================================================== //
// Create a trail of points going in positive x direction. 
// 
// @params
// * unsigned nPoints, number of points
// * float nGap, gap between the points
// * Color first, color of the first segment (near entity)
// * Color last, color of the last segment
// 
// @ return
// pointer to the laser entity
Entity* createLaser(unsigned nPoints, float nGap, Color first, Color last);


// randomColorFrom7
// ========================================================================== //
// Get a random color from a selection of 7.
// WHITE, RED, GREEN, BLUE, TEAL, MAGENTA, YELLOW
// 
// @ return
// The generated color.
Color randomColorFrom7();


// randomOrthogonalVector
// ====================================================================== //
// Get a random unit vector that is perpendicular to this vector.
//
// @param
// * const Vector & v, Vector that the returned vector will be orthogonal
//
// @return
// The othogonal vector.
Vector randomOrthogonalVector(const Vector & v);


// createFlower
// ========================================================================== //
// Creates a flowwer affect entity. (Doesn't set it's location.)
// 
// @params
// * Vector v, A vector normal to the plane on which the flower lies
// * unsigned numberOfLines, number of lines stemming from the center of the
//                           flower shape.
// 
// @return
// pointer to the entity
Entity* createFlower(const Vector & v, unsigned numberOfLines);


// spawnShipBullet
// ========================================================================== //
// Generate a bullet entity getting shot out by the given entity. The bullet
// moves in the direction the given entity is facing. Also the entity is
// assumed to be a ship and the bullet's ID is set accordingly.
//
// @params
// * const Entity * entity, the entity shooting the bullet
// * Color color, color of the bullet
// * float speed, speed of the bullet
// 
// @return
// pointer to the spawned bullet entity
Entity* spawnShipBullet(const Entity * entity, Color color, float speed);


// spawnSaucerBullet
// ========================================================================== //
// Generate a bullet entity getting shot by the source entity out at the 
// target point. The bullet is assumed to be a saucer bullet and it's ID is
// set accordingly.
// 
// @params
// * const Entity * source, the entity shooting the bullet
// * const Point & target, the point the bullet will be moving towards
// * Color color, color of the bullet
// * float speed, speed of the bullet
// 
// @return
// pointer to the spawned bullet entity
Entity* spawnSaucerBullet(const Entity * source, const Point & target, Color color, float speed);


// updateTrailFrame
// ========================================================================== //
// Set all the point locations to be the previous points location and the
// first points location to be the given location.
// 
// @params
// * Frame & frame, the trail frame being modifiled
// * const Point & location, the current new location of the trail
void updateTrailFrame(Frame & frame, const Point & location);


// update_TrailFrame
// ========================================================================== //
// Set all the points to equal the respective previous point location and the
// first point's location to the given entitiy's L or R position. Since the
// trails will be following a ship entity, L is for the left wing and R is
// for the right.
// 
// @params
// * Frame & frame, the trail frame being modifiled
// * const Entity * entity, the entity the trail will follow
void updateRTrailFrame(Frame & frame, const Entity * entity);
void updateLTrailFrame(Frame & frame, const Entity * entity);


// updateLaserFrame
// ========================================================================== //
// Set the points for the given frame to point in the direction the given
// entity is facing.
// 
// @params
// * Frame & frame, the trail frame being modifiled
// * float nGap, gap between the points
// * const Entity * entity, the entity
void updateLaserFrame(Frame & frame, float nGap, const Entity * entity);


// updateFlowerFrame
// ========================================================================== //
// Update the flower effects frame by increasing the length of its lines.
// Makes it look like its expanding out like an explosion.
// 
// @param
// * frame & frame, flower frame
void updateFlowerFrame(Frame & frame);


// ************************************************************************** //
// vvv                             COLLISION                              vvv //
// ************************************************************************** //


// fasterLineTriangleIntersect
// ========================================================================== //
// Checks if a given line segment intersects a given triangle. If it does
// sets the given point to equal the location of the intersect.
// Faster then the lineTriangleIntersect.
// 
// @params
// * const Line & line, the line being checked
// * const Triangle & triangle, the triangle
// * Point & location, location of the intersect
// 
// @return
// true if there is an intersection
bool fasterLineTriangleIntersect(const Line & line, const Triangle & triangle, Point & location);


// pointInsideShapeCheck
// ========================================================================== //
// Check if the given point is inside the given shape using the "even-odd"
// method.
//
// The way that this is done is that a long line is drawn from the point out
// in the x direction. If that line crosses the shape's triangles, an odd
// number of times, it is considered to be inside the shape, if it crosses an
// even number of times, it is considered to be outsize the shape.
// 
// @params
// * const Point & point, point being checked
// * const Shape & shape, shape being checked
// * float length, a large number used to deternime the length of the line
//                 used in the "even-odd" method.
// 
// @return
// True if the given point is inside the given shape.
bool pointInsideShapeCheck(const Point & point, const Shape & shape, float length);


// hardCapSpeed
// ========================================================================== //
// If the given entity's speed is above the given limit, change the given
// entity's speed to the limit.
// 
// @param
// * Entity * entitiy, entity who's speed is being limited
// * float limit, speed limit
void hardCapSpeed(Entity * entity, float limit);


// softCapSpeed
// ========================================================================== //
// If the given entity's speed is above the given limit, decrease it's speed
// by the given deceleration value.
// 
// @param
// * Entity * entitiy, entity who's speed is being limited
// * float limit, speed limit
// * float deceleration, the value by which the speed limit is reduced
void softCapSpeed(Entity * entity, float limit, float deceleration);


// ************************************************************************** //
// vvv                               UNUSED                               vvv //
// ************************************************************************** //


// createBorderAlt
// ========================================================================== //
// Create a border which is made up of many entities.
// 
// @params
// * Array<Entity*> & entities, The array that will be filled with entities
//                              making up the border
// * float r, border radius
// * float n, subdivides determining the number of entities the border will
//            be made up of
// * float m, further subdevides determining how many triangles in each
//            entity
// * Color color, The border's color
void createBorderAlt(Array<Entity*> & entities, float r, float n, float m, Color color);


// lineTriangleIntersect
// ========================================================================== //
// Checks if a given line segment intersects a given triangle. If it does
// sets the given point to equal the location of the intersect.
// 
// @params
// * const Line & line, the line being checked
// * const Triangle & triangle, the triangle
// * Point & location, location of the intersect
// 
// @return
// true if there is an intersection
bool lineTriangleIntersect(const Line & line, const Triangle & triangle, Point & location);


// shapeShapeIntersect
// ========================================================================== //
// Checks if two given shapes intersect. More precisely, checks if any lines
// of the first shape (a), intersect any of the triangles of the second shape
// (b). If they do, the location point is set to the interesect point.
//
// @params
// * const Shape & shapeA, first shape, expected to be in world space and
//                         contain lines
// * const Shape & shapeB, second shape, expected to be in world space and
//                         contain triangles
// * Point & location, location of the intersect
// 
// @return
// true if there is an intersection
bool shapeShapeIntersect(const Shape & shapeA, const Shape & shapeB, Point & location);


// calculateEntityCollisionsSimple
// ========================================================================== //
// Apply collision physics between the given entities. Physics only affects
// velocity.
// 
// @params
// * Array<Entity*> entities, entities having physics applied
void calculateEntityCollisionsSimple(Array<Entity*> entities);


// calculateBorderCollisionsSimple
// ========================================================================== //
// For each of the given entities apply collision physics with the given
// border. Physics only affects velocity.
// 
// @params
// * Array<Entity*> entities, entities being checked
// * Entity * border, border that entities might be colliding with
void calculateBorderCollisionsSimple(Array<Entity*> entities, Entity * border);


// calculateBorderCollisionsOld
// ========================================================================== //
// The old collision function I was using. It sucked...
void calculateBorderCollisionsOld(Array<Entity*> entities, Entity * border);