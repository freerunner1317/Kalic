#pragma once
#include <Arduino.h>
#include <FastLED.h>

uint16_t getPix(int x, int y);
void setPix(int x, int y, CRGB color);
#define FOR_i(x,y)  for (int i = (x); i < (y); i++)
#define FOR_j(x,y)  for (int j = (x); j < (y); j++)
uint16_t NormalizeRange(uint16_t number, uint16_t min, uint16_t max);

void fillMyPal16_2(uint8_t hue, bool isInvert);

void dimAll(uint8_t value);

float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max);
void blurScreen(fract8 blur_amount, CRGB *LEDarray);
uint8_t myScale8(uint8_t x);

uint8_t wrapX(int8_t x);
uint8_t wrapY(int8_t y);

static const uint8_t MBAuroraColors_arr[5][4] PROGMEM = // палитра в формате CHSV
{//№, цвет, насыщенность, яркость
  {0  , 0 , 255,   0},// black
  {80 , 0 , 255, 255},
  {130, 25, 220, 255},
  {180, 25, 185, 255},
  {255, 25, 155, 255} //245
};


// template <class T>
// class Vector2 {
// public:
//     T x, y;

//     Vector2() :x(0), y(0) {}
//     Vector2(T x, T y) : x(x), y(y) {}
//     Vector2(const Vector2& v) : x(v.x), y(v.y) {}

//     Vector2& operator=(const Vector2& v);
    
//     bool isEmpty();

//     bool operator==(Vector2& v);

//     bool operator!=(Vector2& v);

//     Vector2 operator+(Vector2& v);
//     Vector2 operator-(Vector2& v);

//     Vector2& operator+=(Vector2& v) ;
//     Vector2& operator-=(Vector2& v);

//     Vector2 operator+(double s);
//     Vector2 operator-(double s);
//     Vector2 operator*(double s);
//     Vector2 operator/(double s) ;
    
//     Vector2& operator+=(double s);
//     Vector2& operator-=(double s);
//     Vector2& operator*=(double s);
//     Vector2& operator/=(double s);

//     void set(T x, T y) ;

//     void rotate(double deg);

//     Vector2& normalize();

//     float dist(Vector2 v) const ;
//     float length() const ;

//     float mag() const ;

//     float magSq();

//     void truncate(double length);

//     Vector2 ortho() const;

//     static float dot(Vector2 v1, Vector2 v2);
//     static float cross(Vector2 v1, Vector2 v2);

//     void limit(float max);
// };

// typedef Vector2<float> PVector;

// class Boid {
//   public:

//     // PVector location;
//     // PVector velocity;
//     // PVector acceleration;
//     // float maxforce;    // Maximum steering force
//     // float maxspeed;    // Maximum speed

//     // float desiredseparation = 4;
//     // float neighbordist = 8;
//     // byte colorIndex = 0;
//     // float mass;

//     // boolean enabled = true;

//     Boid() {}

//     Boid(float x, float y);

//     static float randomf();

//     static float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

//     void run(Boid boids [], uint8_t boidCount);

//     // Method to update location
//     void update();

//     void applyForce(PVector force);

//     void repelForce(PVector obstacle, float radius);

//     // We accumulate a new acceleration each time based on three rules
//     void flock(Boid boids [], uint8_t boidCount);

//     // Separation
//     // Method checks for nearby boids and steers away
//     PVector separate(Boid boids [], uint8_t boidCount);

//     // Alignment
//     // For every nearby boid in the system, calculate the average velocity
//     PVector align(Boid boids [], uint8_t boidCount);

//     // Cohesion
//     // For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
//     PVector cohesion(Boid boids [], uint8_t boidCount);

//     // A method that calculates and applies a steering force towards a target
//     // STEER = DESIRED MINUS VELOCITY
//     PVector seek(PVector target);

//     // A method that calculates a steering force towards a target
//     // STEER = DESIRED MINUS VELOCITY
//     void arrive(PVector target);

//     void wrapAroundBorders() ;

//     void avoidBorders();

//     bool bounceOffBorders(float bounce);

//     void render();
// };
