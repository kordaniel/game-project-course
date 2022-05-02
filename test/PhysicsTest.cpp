#include "gtest/gtest.h"
#include "gmock/gmock.h" //EXPECT_THAT macro, matchers

#include "Physics.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext/matrix_relational.hpp>


class DerivedPhysicsObj : public PhysicsObject
{
public:
    DerivedPhysicsObj(const glm::vec3& pos, const glm::vec3& vel, const glm::vec3& acc)
        : PhysicsObject(pos, vel, acc) {}
    
    virtual void UpdatePhysics([[maybe_unused]] const Physics& physicsEng,
                               [[maybe_unused]] RectangleF boundaries,
                               [[maybe_unused]] Timestep dt) override
    {
        //
    }
};

TEST(PhysicsObjectTest, ConstructorSetsCorrectPosVelAcceleration)
{
    glm::vec3 pos(1.0f, 2.0f, 0.0f);
    glm::vec3 vel(0.002f, 1231.21f, 0.0f);
    glm::vec3 acc(12.12f, -0.012f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel, acc);
    glm::mat4 expectedAcceleration = glm::translate(glm::mat4(1.0f), acc);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(vel, 1.0f), pObj.GetVelocity())));
    EXPECT_TRUE(glm::all(glm::equal(expectedAcceleration, pObj.GetAcceleration())))
        << "Expected: " << glm::to_string(expectedAcceleration) << '\n'
        << "Actual: " << glm::to_string(pObj.GetAcceleration());
}

TEST(PhysicsObjectTest, ApplyForceWest)
{
    float force = 96.34f;
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 vel(0.0f, 0.0f, 0.0f);
    glm::vec3 acc(0.0f, 0.0f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel, acc);
    acc.x -= force;
    pObj.ApplyForce(Physics::Direction::WEST, force);
    glm::mat4 expectedAcceleration = glm::translate(glm::mat4(1.0f), acc);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(vel, 1.0f), pObj.GetVelocity())));
    EXPECT_TRUE(glm::all(glm::equal(expectedAcceleration, pObj.GetAcceleration())))
        << "Expected: " << glm::to_string(expectedAcceleration) << '\n'
        << "Actual: " << glm::to_string(pObj.GetAcceleration());
}

TEST(PhysicsObjectTest, ApplyForceNorth)
{
    float force = 0.023f;
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 vel(0.0f, 0.0f, 0.0f);
    glm::vec3 acc(0.0f, 0.0f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel, acc);
    acc.y -= force;
    pObj.ApplyForce(Physics::Direction::NORTH, force);
    glm::mat4 expectedAcceleration = glm::translate(glm::mat4(1.0f), acc);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(vel, 1.0f), pObj.GetVelocity())));
    EXPECT_TRUE(glm::all(glm::equal(expectedAcceleration, pObj.GetAcceleration())))
        << "Expected: " << glm::to_string(expectedAcceleration) << '\n'
        << "Actual: " << glm::to_string(pObj.GetAcceleration());
}

TEST(PhysicsObjectTest, ApplyForceEast)
{
    float force = 100.0f;
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 vel(0.0f, 0.0f, 0.0f);
    glm::vec3 acc(0.0f, 0.0f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel, acc);
    acc.x += force;
    pObj.ApplyForce(Physics::Direction::EAST, force);
    glm::mat4 expectedAcceleration = glm::translate(glm::mat4(1.0f), acc);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(vel, 1.0f), pObj.GetVelocity())));
    EXPECT_TRUE(glm::all(glm::equal(expectedAcceleration, pObj.GetAcceleration())))
        << "Expected: " << glm::to_string(expectedAcceleration) << '\n'
        << "Actual: " << glm::to_string(pObj.GetAcceleration());
}

TEST(PhysicsObjectTest, ApplyForceSouth)
{
    float force = 1532.12331f;
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 vel(0.0f, 0.0f, 0.0f);
    glm::vec3 acc(0.0f, 0.0f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel, acc);
    acc.y += force;
    pObj.ApplyForce(Physics::Direction::SOUTH, force);
    glm::mat4 expectedAcceleration = glm::translate(glm::mat4(1.0f), acc);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(vel, 1.0f), pObj.GetVelocity())));
    EXPECT_TRUE(glm::all(glm::equal(expectedAcceleration, pObj.GetAcceleration())))
        << "Expected: " << glm::to_string(expectedAcceleration) << '\n'
        << "Actual: " << glm::to_string(pObj.GetAcceleration());
}

TEST(PhysicsObjectTest, ApplyForceAngles)
{
    for (size_t angle = 0; angle <= 360; angle += 45)
    {
        const float angleF = static_cast<float>(angle);
        const float forceF = 100.0f;

        glm::vec3 pos(0.0f, 0.0f, 0.0f);
        glm::vec3 vel(0.0f, 0.0f, 0.0f);
        glm::vec3 acc(0.0f, 0.0f, 0.0f);
        DerivedPhysicsObj pObj(pos, vel, acc);
        pObj.ApplyForce(angleF, forceF);

        glm::mat4 rotation(1.0f);
        rotation = glm::rotate(rotation, glm::radians(angleF), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec4 translationVec = rotation * glm::vec4(0.0f, -forceF, 0.0f, 1.0f);

        glm::mat4 expectedAcceleration = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(translationVec.x, translationVec.y, translationVec.z)
        );

        EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())))
            << "Position is incorrect after applying a force towards an angle of " << angle << " degrees.";
        EXPECT_TRUE(glm::all(glm::equal(glm::vec4(vel, 1.0f), pObj.GetVelocity())))
            << "Velocity is incorrect after applying a force towards an angle of " << angle << " degrees.";
        EXPECT_TRUE(glm::all(glm::equal(expectedAcceleration, pObj.GetAcceleration())))
            << "Velocity is incorrect after applying a force towards an angle of " << angle << " degrees.\n"
            << "Expected: " << glm::to_string(expectedAcceleration) << '\n'
            << "Actual: " << glm::to_string(pObj.GetAcceleration()) << std::endl;
    }
}
