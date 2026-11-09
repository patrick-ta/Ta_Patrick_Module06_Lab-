#pragma once
#include <cmath>
#include <cstdint>
namespace Week06 {
enum class Action { Ready, Attacking, Hurt, Dead };
enum class Visual { Idle, Run, Air, Attack, Hurt, Dead };
struct Model {
 Action State=Action::Ready;
 double Started=0, LastTime=0;
 double Startup=.10, Active=.05, Recovery=.20, HurtTime=.30;
 std::uint64_t Generation=0;
 bool Committed=false;
 bool Clock(double now) { if(!std::isfinite(now)||now<0||now<LastTime)return false; LastTime=now;return true; }
 bool Request(double now) {
  if(!Clock(now)||State!=Action::Ready)return false;
  State=Action::Attacking;Started=now;Committed=false;++Generation;return true;
 }
 bool Step(double now) {
  if(!Clock(now))return false;
  bool fire=false;
  if(State==Action::Attacking) {
   const double elapsed=now-Started;
   // LAB 1: consume the active threshold once, including a frame crossing recovery.
   // TODO LAB 1
   (void)elapsed;
   if(elapsed>=Startup+Active+Recovery)State=Action::Ready;
  } else if(State==Action::Hurt && now-Started>=HurtTime)State=Action::Ready;
  return fire;
 }
 void Interrupt(double now,bool dead) {
  if(!Clock(now)||State==Action::Dead)return;
  // LAB 2: invalidate old work and commit Hurt or terminal Dead.
  // TODO LAB 2
  (void)dead;
 }
 bool Complete(std::uint64_t token,double now) {
  // LAB 3: stale/dead completions must leave state and clock untouched.
  // TODO LAB 3: implement guards before the supplied clock/state update.
  (void)token; return false;
  if(!Clock(now))return false;State=Action::Ready;return true;
 }
 Visual Select(bool falling,double speed) const {
  // LAB 4: visual selection reads action, grounding and speed without mutating state.
  // TODO LAB 4
  (void)falling;(void)speed;return Visual::Idle;
 }
};
inline bool ChangeVisual(int current,int desired) { return current!=desired; }
}
