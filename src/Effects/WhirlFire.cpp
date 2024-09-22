// #include <WS2812.h>
// #include <FastLED.h>
// #include <constants.h>
// #include <MyMatrix.h>
// #include <EffectsManager.h>
// #include <types.h>
// #include <Effects/WhirlFire.h>
// #include <palettes.h>
// #include <CommonFunc.h>

// extern uint16_t ff_x, ff_y, ff_z;                      // большие счётчики

// template <class T>
// class Vector2 {
// public:
//     T x, y;

//     Vector2() :x(0), y(0) {}
//     Vector2(T x, T y) : x(x), y(y) {}
//     Vector2(const Vector2& v) : x(v.x), y(v.y) {}

//     Vector2& operator=(const Vector2& v) {
//         x = v.x;
//         y = v.y;
//         return *this;
//     }
    
//     bool isEmpty() {
//         return x == 0 && y == 0;
//     }

//     bool operator==(Vector2& v) {
//         return x == v.x && y == v.y;
//     }

//     bool operator!=(Vector2& v) {
//         return !(x == y);
//     }

//     Vector2 operator+(Vector2& v) {
//         return Vector2(x + v.x, y + v.y);
//     }
//     Vector2 operator-(Vector2& v) {
//         return Vector2(x - v.x, y - v.y);
//     }

//     Vector2& operator+=(Vector2& v) {
//         x += v.x;
//         y += v.y;
//         return *this;
//     }
//     Vector2& operator-=(Vector2& v) {
//         x -= v.x;
//         y -= v.y;
//         return *this;
//     }

//     Vector2 operator+(double s) {
//         return Vector2(x + s, y + s);
//     }
//     Vector2 operator-(double s) {
//         return Vector2(x - s, y - s);
//     }
//     Vector2 operator*(double s) {
//         return Vector2(x * s, y * s);
//     }
//     Vector2 operator/(double s) {
//         return Vector2(x / s, y / s);
//     }
    
//     Vector2& operator+=(double s) {
//         x += s;
//         y += s;
//         return *this;
//     }
//     Vector2& operator-=(double s) {
//         x -= s;
//         y -= s;
//         return *this;
//     }
//     Vector2& operator*=(double s) {
//         x *= s;
//         y *= s;
//         return *this;
//     }
//     Vector2& operator/=(double s) {
//         x /= s;
//         y /= s;
//         return *this;
//     }

//     void set(T x, T y) {
//         this->x = x;
//         this->y = y;
//     }

//     void rotate(double deg) {
//         double theta = deg / 180.0 * M_PI;
//         double c = cos(theta);
//         double s = sin(theta);
//         double tx = x * c - y * s;
//         double ty = x * s + y * c;
//         x = tx;
//         y = ty;
//     }

//     Vector2& normalize() {
//         if (length() == 0) return *this;
//         *this *= (1.0 / length());
//         return *this;
//     }

//     float dist(Vector2 v) const {
//         Vector2 d(v.x - x, v.y - y);
//         return d.length();
//     }
//     float length() const {
//         //return SQRT_VARIANT(x * x + y * y); некорректно работает через sqrt3, нужно sqrt
//         return sqrt(x * x + y * y);
//     }

//     float mag() const {
//         return length();
//     }

//     float magSq() {
//         return (x * x + y * y);
//     }

//     void truncate(double length) {
//         double angle = atan2f(y, x);
//         x = length * cos(angle);
//         y = length * sin(angle);
//     }

//     Vector2 ortho() const {
//         return Vector2(y, -x);
//     }

//     static float dot(Vector2 v1, Vector2 v2) {
//         return v1.x * v2.x + v1.y * v2.y;
//     }
//     static float cross(Vector2 v1, Vector2 v2) {
//         return (v1.x * v2.y) - (v1.y * v2.x);
//     }

//     void limit(float max) {
//         if (magSq() > max*max) {
//             normalize();
//             *this *= max;
//         }
//     }
// };

// typedef Vector2<float> PVector;

// class Boid {
//   public:

//     PVector location;
//     PVector velocity;
//     PVector acceleration;
//     float maxforce;    // Maximum steering force
//     float maxspeed;    // Maximum speed

//     float desiredseparation = 4;
//     float neighbordist = 8;
//     byte colorIndex = 0;
//     float mass;

//     boolean enabled = true;

//     Boid() {}

//     Boid(float x, float y) {
//       acceleration = PVector(0, 0);
//       velocity = PVector(randomf(), randomf());
//       location = PVector(x, y);
//       maxspeed = 1.5;
//       maxforce = 0.05;
//     }

//     static float randomf() {
//       return mapfloat(random(0, 255), 0, 255, -.5, .5);
//     }

//     static float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
//       return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
//     }

//     void run(Boid boids [], uint8_t boidCount) {
//       flock(boids, boidCount);
//       update();
//       // wrapAroundBorders();
//       // render();
//     }

//     // Method to update location
//     void update() {
//       // Update velocity
//       velocity += acceleration;
//       // Limit speed
//       velocity.limit(maxspeed);
//       location += velocity;
//       // Reset acceleration to 0 each cycle
//       acceleration *= 0;
//     }

//     void applyForce(PVector force) {
//       // We could add mass here if we want A = F / M
//       acceleration += force;
//     }

//     void repelForce(PVector obstacle, float radius) {
//       //Force that drives boid away from obstacle.

//       PVector futPos = location + velocity; //Calculate future position for more effective behavior.
//       PVector dist = obstacle - futPos;
//       float d = dist.mag();

//       if (d <= radius) {
//         PVector repelVec = location - obstacle;
//         repelVec.normalize();
//         if (d != 0) { //Don't divide by zero.
//           // float scale = 1.0 / d; //The closer to the obstacle, the stronger the force.
//           repelVec.normalize();
//           repelVec *= (maxforce * 7);
//           if (repelVec.mag() < 0) { //Don't let the boids turn around to avoid the obstacle.
//             repelVec.y = 0;
//           }
//         }
//         applyForce(repelVec);
//       }
//     }

//     // We accumulate a new acceleration each time based on three rules
//     void flock(Boid boids [], uint8_t boidCount) {
//       PVector sep = separate(boids, boidCount);   // Separation
//       PVector ali = align(boids, boidCount);      // Alignment
//       PVector coh = cohesion(boids, boidCount);   // Cohesion
//       // Arbitrarily weight these forces
//       sep *= 1.5;
//       ali *= 1.0;
//       coh *= 1.0;
//       // Add the force vectors to acceleration
//       applyForce(sep);
//       applyForce(ali);
//       applyForce(coh);
//     }

//     // Separation
//     // Method checks for nearby boids and steers away
//     PVector separate(Boid boids [], uint8_t boidCount) {
//       PVector steer = PVector(0, 0);
//       int count = 0;
//       // For every boid in the system, check if it's too close
//       for (int i = 0; i < boidCount; i++) {
//         Boid other = boids[i];
//         if (!other.enabled)
//           continue;
//         float d = location.dist(other.location);
//         // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
//         if ((d > 0) && (d < desiredseparation)) {
//           // Calculate vector pointing away from neighbor
//           PVector diff = location - other.location;
//           diff.normalize();
//           diff /= d;        // Weight by distance
//           steer += diff;
//           count++;            // Keep track of how many
//         }
//       }
//       // Average -- divide by how many
//       if (count > 0) {
//         steer /= (float) count;
//       }

//       // As long as the vector is greater than 0
//       if (steer.mag() > 0) {
//         // Implement Reynolds: Steering = Desired - Velocity
//         steer.normalize();
//         steer *= maxspeed;
//         steer -= velocity;
//         steer.limit(maxforce);
//       }
//       return steer;
//     }

//     // Alignment
//     // For every nearby boid in the system, calculate the average velocity
//     PVector align(Boid boids [], uint8_t boidCount) {
//       PVector sum = PVector(0, 0);
//       int count = 0;
//       for (int i = 0; i < boidCount; i++) {
//         Boid other = boids[i];
//         if (!other.enabled)
//           continue;
//         float d = location.dist(other.location);
//         if ((d > 0) && (d < neighbordist)) {
//           sum += other.velocity;
//           count++;
//         }
//       }
//       if (count > 0) {
//         sum /= (float) count;
//         sum.normalize();
//         sum *= maxspeed;
//         PVector steer = sum - velocity;
//         steer.limit(maxforce);
//         return steer;
//       }
//       else {
//         return PVector(0, 0);
//       }
//     }

//     // Cohesion
//     // For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
//     PVector cohesion(Boid boids [], uint8_t boidCount) {
//       PVector sum = PVector(0, 0);   // Start with empty vector to accumulate all locations
//       int count = 0;
//       for (int i = 0; i < boidCount; i++) {
//         Boid other = boids[i];
//         if (!other.enabled)
//           continue;
//         float d = location.dist(other.location);
//         if ((d > 0) && (d < neighbordist)) {
//           sum += other.location; // Add location
//           count++;
//         }
//       }
//       if (count > 0) {
//         sum /= count;
//         return seek(sum);  // Steer towards the location
//       }
//       else {
//         return PVector(0, 0);
//       }
//     }

//     // A method that calculates and applies a steering force towards a target
//     // STEER = DESIRED MINUS VELOCITY
//     PVector seek(PVector target) {
//       PVector desired = target - location;  // A vector pointing from the location to the target
//       // Normalize desired and scale to maximum speed
//       desired.normalize();
//       desired *= maxspeed;
//       // Steering = Desired minus Velocity
//       PVector steer = desired - velocity;
//       steer.limit(maxforce);  // Limit to maximum steering force
//       return steer;
//     }

//     // A method that calculates a steering force towards a target
//     // STEER = DESIRED MINUS VELOCITY
//     void arrive(PVector target) {
//       PVector desired = target - location;  // A vector pointing from the location to the target
//       float d = desired.mag();
//       // Normalize desired and scale with arbitrary damping within 100 pixels
//       desired.normalize();
//       if (d < 4) {
//         float m = map(d, 0, 100, 0, maxspeed);
//         desired *= m;
//       }
//       else {
//         desired *= maxspeed;
//       }

//       // Steering = Desired minus Velocity
//       PVector steer = desired - velocity;
//       steer.limit(maxforce);  // Limit to maximum steering force
//       applyForce(steer);
//       //Serial.println(d);
//     }

//     void wrapAroundBorders() {
//       if (location.x < 0) location.x = WIDTH - 1;
//       if (location.y < 0) location.y = HEIGHT - 1;
//       if (location.x >= WIDTH) location.x = 0;
//       if (location.y >= HEIGHT) location.y = 0;
//     }

//     void avoidBorders() {
//       PVector desired = velocity;

//       if (location.x < 8) desired = PVector(maxspeed, velocity.y);
//       if (location.x >= WIDTH - 8) desired = PVector(-maxspeed, velocity.y);
//       if (location.y < 8) desired = PVector(velocity.x, maxspeed);
//       if (location.y >= HEIGHT - 8) desired = PVector(velocity.x, -maxspeed);

//       if (desired != velocity) {
//         PVector steer = desired - velocity;
//         steer.limit(maxforce);
//         applyForce(steer);
//       }

//       if (location.x < 0) location.x = 0;
//       if (location.y < 0) location.y = 0;
//       if (location.x >= WIDTH) location.x = WIDTH - 1;
//       if (location.y >= HEIGHT) location.y = HEIGHT - 1;
//     }

//     bool bounceOffBorders(float bounce) {
//       bool bounced = false;

//       if (location.x >= WIDTH) {
//         location.x = WIDTH - 1;
//         velocity.x *= -bounce;
//         bounced = true;
//       }
//       else if (location.x < 0) {
//         location.x = 0;
//         velocity.x *= -bounce;
//         bounced = true;
//       }

//       if (location.y >= HEIGHT) {
//         location.y = HEIGHT - 1;
//         velocity.y *= -bounce;
//         bounced = true;
//       }
//       else if (location.y < 0) {
//         location.y = 0;
//         velocity.y *= -bounce;
//         bounced = true;
//       }

//       return bounced;
//     }

//     void render() {
//       // // Draw a triangle rotated in the direction of velocity
//       // float theta = velocity.heading2D() + radians(90);
//       // fill(175);
//       // stroke(0);
//       // pushMatrix();
//       // translate(location.x,location.y);
//       // rotate(theta);
//       // beginShape(TRIANGLES);
//       // vertex(0, -r*2);
//       // vertex(-r, r*2);
//       // vertex(r, r*2);
//       // endShape();
//       // popMatrix();
//       // backgroundLayer.drawPixel(location.x, location.y, CRGB::Blue);
//     }
// };

// static const uint8_t AVAILABLE_BOID_COUNT = 20U;
// Boid boids[AVAILABLE_BOID_COUNT]; 

// const TProgmemRGBPalette16 *curPaletteWhirl = palette_arr[0];

// void whirlRoutine() {

//     //setCurrentPalette();
//     curPaletteWhirl = palette_arr[(uint8_t)(1/100.0F*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-0.01F))];
//     ff_x = random16();
//     ff_y = random16();
//     ff_z = random16();

//     // for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
//     //     boids[i] = Boid(random8(WIDTH), 0);
//     // }

//     // dimAll(240);

//     // for (uint8_t i = 0; i < AVAILABLE_BOID_COUNT; i++) {
//     //     Boid * boid = &boids[i];

//     //     int ioffset = ff_scale * boid->location.x;
//     //     int joffset = ff_scale * boid->location.y;

//     //     byte angle = inoise8(ff_x + ioffset, ff_y + joffset, ff_z);

//     //     boid->velocity.x = (float) sin8(angle) * 0.0078125 - 1.0;
//     //     boid->velocity.y = -((float)cos8(angle) * 0.0078125 - 1.0);
//     //     boid->update();

//     //     //if (oneColor)
//     //         //drawPixelXY(boid->location.x, boid->location.y, CHSV(modes[currentMode].Scale * 2.55, (modes[currentMode].Scale == 100) ? 0U : 255U, 255U)); // цвет белый для .Scale=100
//     //         drawPixelXYF(boid->location.x, boid->location.y, CHSV(2.55, 0, 255U)); // цвет белый для .Scale=100
//     //     //else
//     //         //drawPixelXY(boid->location.x, boid->location.y, ColorFromPalette(*curPalette, angle + hue)); // + hue постепенно сдвигает палитру по кругу
//     //         //drawPixelXYF(boid->location.x, boid->location.y, ColorFromPalette(*curPalette, angle + hue)); // + hue постепенно сдвигает палитру по кругу

//     //     if (boid->location.x < 0 || boid->location.x >= WIDTH || boid->location.y < 0 || boid->location.y >= HEIGHT) {
//     //         boid->location.x = random(WIDTH);
//     //         boid->location.y = 0;
//     //     }
//     // }
//     // ff_x += ff_speed;
//     // ff_y += ff_speed;
//     // ff_z += ff_speed;
// }

