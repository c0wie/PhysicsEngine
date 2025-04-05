#pragma once

// local
#include "math.hpp"
#include "vector2.hpp"

// std
#include <array>
#include <stdexcept>

namespace pe2d {

/////////////////////////////////////////////////////////////////////
/// @brief Struct representing necessary information about collision.
/// @details If two boxes are colliding with they faces the collision
///          points are consider to be two most extreme points in each
///          direction. If the collision is happening in single point
///          the point is saved in contact_point1.
/////////////////////////////////////////////////////////////////////
struct CollisionPoints {
public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Default constructor sets normal to (0.0, 0.0), depth to 0.0,
  ///        contact_count to 0, contact_point1 to 0.0 and
  ///        contact_point2 to 0.0.
  /////////////////////////////////////////////////////////////////////
  CollisionPoints() = default;

  /////////////////////////////////////////////////////////////////////
  /// @brief Constructor creating CollisionPoints with given parameters, sets
  /// contact_count to 2.
  /// @param normal - collison normal
  /// @param depth - depth of objects penetration, must be greater or equel to
  /// 0.0
  /// @param contact_point1 - first contact point
  /// @param contact_point2 - second contact point
  /// @throw std::invalid_argument if depth is negative.
  /// @throw std::invalid_argument if normal is not a unit vector.
  /////////////////////////////////////////////////////////////////////
  CollisionPoints(Vec2f normal, float depth, Vec2f contact_point1,
                  Vec2f contact_point2)
      : normal(normal), contact_point1(contact_point1),
        contact_point2(contact_point2), depth(depth), contact_count(2) {
    if (depth < 0.0) {
      throw std::invalid_argument("[CollisionPoints::CollisionPoints()] Error: "
                                  "depth of collision musn't be negative"
                                  "(received: " +
                                  std::to_string(depth) + ")");
    }
    if (!math::NearlyEquel(math::Length(normal), 1.0f) ) {
      throw std::invalid_argument("[CollisionPoints::CollisionPoints()] Error: "
                                  "normal must be unit length vector"
                                  "(received: " +
                                  normal.GetString() + ")");
    }
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Constructor creating CollisionPoints with given parameters, sets
  /// contact_count to 1
  ///        and contact_point2 to default value.
  /// @param normal - collison normal
  /// @param depth - depth of objects penetration, must be greater or equel to
  /// 0.0
  /// @param contact_point1 - first contact point
  /// @throw std::invalid_argument if depth is negative.
  /// @throw std::invalid_argument if normal is not a unit vector.
  /////////////////////////////////////////////////////////////////////
  CollisionPoints(Vec2f normal, float depth, Vec2f contact_point1)
      : normal(normal), contact_point1(contact_point1), depth(depth),
        contact_count(1) {
    if (depth < 0.0) {
      throw std::invalid_argument("[CollisionPoints::CollisionPoints()] Error: "
                                  "depth of collision musn't be negative"
                                  "(received: " +
                                  std::to_string(depth) + ")");
    }
    if (!math::NearlyEquel(math::Length(normal), 1.0f)) {
      throw std::invalid_argument("[CollisionPoints::CollisionPoints()] Error: "
                                  "normal must be unit length vector"
                                  "(received: " +
                                  normal.GetString() + ")");
    }
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Getter giving both contact points at once.
  /// @details Useful for iterating over contact points. When contact_count is
  ///          1, the second point is set to default value.
  /// @return Array containing both contact points.
  /////////////////////////////////////////////////////////////////////
  std::array<Vec2f, 2> GetContactPoints() const {return {contact_point1, contact_point2};}
public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Unit length vector representing direction of collision
  /////////////////////////////////////////////////////////////////////
  Vec2f normal{0.0f, 0.0f};

  /////////////////////////////////////////////////////////////////////
  /// @brief Scalar informating how much objects are intersecting along normal
  /////////////////////////////////////////////////////////////////////
  float depth{0.0f};

  /////////////////////////////////////////////////////////////////////
  /// @brief Number of valid contact points
  /////////////////////////////////////////////////////////////////////
  unsigned int contact_count{0};

  /////////////////////////////////////////////////////////////////////
  /// @brief Primary contact point
  /////////////////////////////////////////////////////////////////////
  Vec2f contact_point1;

  /////////////////////////////////////////////////////////////////////
  /// @brief Secondary contact point
  /// @details Only valid when contact_count == 2.
  /////////////////////////////////////////////////////////////////////
  Vec2f contact_point2;
};
} // namespace pe2d