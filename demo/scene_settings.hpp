#pragma once

enum SolverType {POSITION_SOLVER, IMPULSE_SOLVER};
struct SceneSettings {
  bool IsGridOn{true};
  bool DrawTangentLines{false};
  bool DrawNormalLines{false};
  bool DrawImpulses{false};
  bool DrawContactPoints{false};
  SolverType Solver{IMPULSE_SOLVER};
};