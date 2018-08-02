/* ==========================================================================
   >File: Game.h
   >Date: 20180713
   >Author: Vik Pandher
   >Details: Here is the bulk of the game's structure.
   ========================================================================== */

#pragma once

#include "ScreenBuffer.h"


// ScreenBuffer from Win32Main.cpp
extern ScreenBuffer * g_screenBuffer;

// The game is a state machine
enum GameState
{
    GS_MAIN,
    GS_PLAY,
    GS_PAUSE,
    GS_END,
    ///GS_SCORES,
    GS_CONTROLS,
    GS_KEY_CHANGE
};

static GameState s_gameState;
static unsigned long long s_gameCounter;

// key press controls
static Array<uint8_t> s_keyKeys;
static Array<Button*> s_keyButtons;
static const unsigned KEY_INDEX_ARROW_LEFT = 0;
static const unsigned KEY_INDEX_ARROW_UP = 1;
static const unsigned KEY_INDEX_ARROW_RIGHT = 2;
static const unsigned KEY_INDEX_ARROW_DOWN = 3;
static const unsigned KEY_INDEX_ENTER = 4;
static const unsigned KEY_INDEX_ROLL_LEFT = 5;
static const unsigned KEY_INDEX_ROLL_RIGHT = 6;
static const unsigned KEY_INDEX_YAW_LEFT = 7;
static const unsigned KEY_INDEX_YAW_RIGHT = 8;
static const unsigned KEY_INDEX_PITCH_UP = 9;
static const unsigned KEY_INDEX_PITCH_DOWN = 10;
static const unsigned KEY_INDEX_ACCELERATE = 11;
static const unsigned KEY_INDEX_SHOOT = 12;
static const unsigned KEY_INDEX_ZOOM = 13;
static const unsigned KEY_INDEX_PAUSE = 14;

static const uint8_t DEFAULT_ROLL_LEFT = 0x51;  // Q
static const uint8_t DEFAULT_ROLL_RIGHT = 0x45; // E
static const uint8_t DEFAULT_YAW_LEFT = 0x41;   // A
static const uint8_t DEFAULT_YAW_RIGHT = 0x44;  // D
static const uint8_t DEFAULT_PITCH_UP = 0x57;   // W
static const uint8_t DEFAULT_PITCH_DOWN = 0x53; // S
static const uint8_t DEFAULT_ACCELERATE = VK_SPACE;
static const uint8_t DEFAULT_SHOOT = VK_OEM_COMMA;
static const uint8_t DEFAULT_ZOOM = VK_SHIFT;


// element colors
static Color s_borderColor = COLOR_GREEN;
static Color s_shipColor0 = COLOR_GREEN;
static Color s_shipColor1 = COLOR_BLUE;
static Color s_shipColor2 = COLOR_RED;
static Color s_trailColor0 = COLOR_ORANGE;
static Color s_trailColor1 = COLOR_YELLOW;
static Color s_laserColor0 = COLOR_RED;
static Color s_laserColor1 = COLOR_YELLOW;
static Color s_asteroidColor = COLOR_WHITE;
static Color s_saucerColor0 = COLOR_YELLOW;
static Color s_saucerColor1 = COLOR_TEAL;
static Color s_shipBulletColor = COLOR_RED;
static Color s_saucerBulletColor = COLOR_MAGENTA;


static const String AUTHOR_STRING("VIK PANDHER");
static const String VERSION_STRING("v1.0");


// main menu elements
static Camera * s_mainCamera;
static Entity * s_mainEntity;

static int s_mainCursor;
static TextBox * s_mainTitle0;
static TextBox * s_mainTitle1;
static Array<TextButton*> s_mainButtons;
static const unsigned MAIN_INDEX_START = 0;
///static const unsigned MAIN_INDEX_SCORES = 
static const unsigned MAIN_INDEX_CONTROLS = 1;

// play menu elements
static const float PLAY_BORDER_RADIUS = 50;
static const float PLAY_CAMERA_DISTANCE = 20;

static Camera * s_playCamera;
static bool s_zoomed;

static Entity * s_playBorder;
static Entity * s_playShip;
static Array<Entity*> s_playAsteroids;
static Array<Entity*> s_playSaucers;
static Array<Entity*> s_playBullets;

static Entity * s_playTrailR;
static Entity * s_playTrailL;
static Entity * s_playLaser;

static Array<Entity*> s_playFlowers;

static const int SHIP_HEALTH = 3;
static const float SHIP_ACCELERATION = 0.02;
static const float SHIP_ROTATION = _PI / 32;
static const float ZOOM_ROTATION = _PI / 64;
static const float SHIP_BULLET_SPEED_LIMIT = 3;
static const float SAUCER_BULLET_SPEED_LIMIT = 2;
static const float HARD_SPEED_LIMIT = 1;
static const float SOFT_SPEED_LIMIT = 0.5;
static const float LIMIT_DECELERATION = 0.01;
static const int SAUCER_FIRE_RATE = 40;

// entities will be deleated once their counter hits zero.
static Array<Entity*> s_limboEntities;
static Array<int> s_limboCounters;

static int s_score;
static int s_scoreTillNectHeart;

// pause menu elements
static int s_pauseCursor;
static Array<TextButton*> s_pauseButtons;
static const unsigned PAUSE_INDEX_RESUME = 0;
static const unsigned PAUSE_INDEX_QUIT = 1;

// end menu elements
static int s_endCursor;
static Array<TextButton*> s_endButtons;
static const unsigned END_INDEX_BACK = 0;

// scores menu elements
///static int s_scoresCursor = 0;
///static Array<TextButton*> s_scoresButtons;
///static const unsigned SCORES_INDEX_BACK = 0;

// controls menu elements
static int s_controlsCursor;
static Array<TextBox*> s_controlsBoxes;
static const unsigned CONTROLS_INDEX_ROLL_LEFT_BOX = 0;
static const unsigned CONTROLS_INDEX_ROLL_RIGHT_BOX = 1;
static const unsigned CONTROLS_INDEX_YAW_LEFT_BOX = 2;
static const unsigned CONTROLS_INDEX_YAW_RIGHT_BOX = 3;
static const unsigned CONTROLS_INDEX_PITCH_UP_BOX = 4;
static const unsigned CONTROLS_INDEX_PITCH_DOWN_BOX = 5;
static const unsigned CONTROLS_INDEX_ACCELERATE_BOX = 6;
static const unsigned CONTROLS_INDEX_SHOOT_BOX = 7;
static Array<TextButton*> s_controlsButtons;
static const unsigned CONTROLS_INDEX_BACK = 0;
static const unsigned CONTROLS_INDEX_ROLL_LEFT = 1;
static const unsigned CONTROLS_INDEX_ROLL_RIGHT = 2;
static const unsigned CONTROLS_INDEX_YAW_LEFT = 3;
static const unsigned CONTROLS_INDEX_YAW_RIGHT = 4;
static const unsigned CONTROLS_INDEX_PITCH_UP = 5;
static const unsigned CONTROLS_INDEX_PITCH_DOWN = 6;
static const unsigned CONTROLS_INDEX_ACCELERATE = 7;
static const unsigned CONTROLS_INDEX_SHOOT = 8;
static const unsigned CONTROLS_INDEX_ZOOM = 9;
static const unsigned CONTROLS_INDEX_DEFAULT = 10;

// key change menu
static Button * s_returnFromKeyChangeButton;


// initialize
// ========================================================================== //
// Initialize many of the game elements.
void initialize();


// deinitialize
// ========================================================================== //
// Clean up what needs to be cleaned up.
void deinitialize();


// keyDown
// ========================================================================== //
// Preform actions that pertain to the key going down.
//
// @param
// * uint8_t k, windows virtual key code
void keyDown(const uint8_t k);


// keyUp
// ========================================================================== //
// Preform actions that pertain to the key going up.
//
// @param
// * uint8_t k, windows virtual key code
void keyUp(const uint8_t k);


// update
// =========================================================================== //
// Update game elements.
void update();


// draw
// ========================================================================== //
// Draw everything to the screen buffer.
void draw();


// changeGameStateTo____
// ========================================================================== //
// Simply change the current game state.
void changeGameStateToMain();
void changeGameStateToPlay();
void changeGameStateToPause();
void changeGameStateToEnd();
///void changeGameStateToScores();
void changeGameStateToControls();
void changeGameStateToKeyChange();


// ____KeyAction
// ========================================================================== //
// Actions for the control key buttons.
void arrowLeftKeyAction();
void arrowUpKeyAction();
void arrowRightKeyAction();
void arrowDownKeyAction();
void enterKeyAction();
void rollLeftKeyAction();
void rollRightKeyAction();
void yawLeftKeyAction();
void yawRightKeyAction();
void pitchUpKeyAction();
void pitchDownKeyAction();
void accelerateKeyAction();
void shootKeyAction();
void zoomKeyAction();
void pauseKeyAction();


// setKeysToDefaults
// ========================================================================== //
// Sets up the default ship controls.
void setKeysToDefaults();

// addToLimbo
// ========================================================================== //
// Add a pointer to the given entity to s_limboEntities, and the
// corresponding counter value to s_limboCounters. If the pointer to the
// entity already exists in s_limboEntities, don't add it and its counter
// value.
// 
// @params
// * Entity * entity, entity pointer being added to limbo
// * int counter, coutner for the entity being added to limbo
void addToLimbo(Entity * entity, int counter);

// deleteFromLimbo
// ========================================================================== //
// Removes the entity pointer from the s_limboEntities, removes it's counter
// from s_limboCounters, removes the entity pointer from its original array,
// and deletes the entity proper with the "delete" call.
// 
// @params
// * int limboIndex, the entity's index in s_limboEntities and s_limbCounters
// * Array<Entity*> & searchEntities, the entity's original array
void deleteFromLimbo(int limboIndex, Array<Entity*> & searchEntities);


// calculateEntityCollisions
// ========================================================================== //
// Apply collision physics between the given entities. Physics is applied;
// entity's are placed into limbo.
// 
// @params
// * Array<Entity*> entities, entities having physics applied
void calculateEntityCollisions(Array<Entity*> & entities);


// calculateBorderCollisions
// ========================================================================== //
// For each of the given entities apply collision physics with the given
// border. Physics ois applied; entity's are placed into limbo.
// 
// @params
// * Array<Entity*> entities, entities being checked
// * Entity * border, border that entities might be colliding with
void calculateBorderCollisions(Array<Entity*> & entities);


// setSpawnLocation
// ========================================================================== //
// Set point to random location located outside the border.
// 
// @param
// Point & location, point being set to the locatoin
void setSpawnLocation(Point & location);


// triggerEntitySpawner
// ========================================================================== //
// Triggers entity spawns depending on a counter value.
void triggerEntitySpawner();

