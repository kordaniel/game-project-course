#include "gtest/gtest.h"
#include "gmock/gmock.h" //EXPECT_THAT macro, matchers

#include "Physics.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>


class DerivedPhysicsObj : public PhysicsObject
{
public:
    DerivedPhysicsObj(const glm::vec3& pos, const glm::vec3& vel)
        : PhysicsObject(pos, vel) {}
    
    virtual void UpdatePhysics([[maybe_unused]] const Physics& physics, [[maybe_unused]] Timestep dt) override
    {
        //
    }
};

TEST(PhysicsObjectTest, ConstructorSetsCorrectPosAndVelocity)
{
    glm::vec3 pos(1.0f, 2.0f, 0.0f);
    glm::vec3 vel(0.002f, 1231.21f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(vel, 1.0f), pObj.GetVelocity())));
}

TEST(PhysicsObjectTest, ApplyForceWest)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 vel(0.0f, 0.0f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel);
    pObj.ApplyForce(Physics::Direction::WEST, 100.0f, 1.0f);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(-100.0f, 0.0f, 0.0f, 1.0f), pObj.GetVelocity())));
}

TEST(PhysicsObjectTest, ApplyForceNorth)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 vel(0.0f, 0.0f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel);
    pObj.ApplyForce(Physics::Direction::NORTH, 100.0f, 1.0f);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(0.0f, -100.0f, 0.0f, 1.0f), pObj.GetVelocity())));
}

TEST(PhysicsObjectTest, ApplyForceEast)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 vel(0.0f, 0.0f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel);
    pObj.ApplyForce(Physics::Direction::EAST, 100.0f, 1.0f);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(100.0f, 0.0f, 0.0f, 1.0f), pObj.GetVelocity())));
}

TEST(PhysicsObjectTest, ApplyForceSouth)
{
    glm::vec3 pos(0.0f, 0.0f, 0.0f);
    glm::vec3 vel(0.0f, 0.0f, 0.0f);
    DerivedPhysicsObj pObj(pos, vel);
    pObj.ApplyForce(Physics::Direction::SOUTH, 100.0f, 1.0f);

    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(pos, 1.0f), pObj.GetPosition())));
    EXPECT_TRUE(glm::all(glm::equal(glm::vec4(0.0f, 100.0f, 0.0f, 1.0f), pObj.GetVelocity())));
}

TEST(PhysicsObjectTest, ApplyForceAngles)
{
    for (size_t angle = 0; angle <= 360; angle += 45)
    {
        const float angleF = static_cast<float>(angle);
        const float forceF = 100.0f;

        glm::vec4 pos(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 vel(0.0f, 0.0f, 0.0f, 1.0f);
        DerivedPhysicsObj pObj(pos, vel);
        pObj.ApplyForce(angleF, forceF, 1.0f);

        glm::mat4 rotation(1.0f);
        rotation = glm::rotate(rotation, glm::radians(angleF), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec4 expectedVel = rotation * glm::vec4(0.0f, -forceF, 0.0f, 1.0f);

        EXPECT_TRUE(glm::all(glm::equal(pos, pObj.GetPosition())))
            << "Position is wrong after applying a force towards an angle of " << angle << " degrees.";
        EXPECT_TRUE(glm::all(glm::equal(expectedVel, pObj.GetVelocity())))
            << "Velocity is wrong after applying a force towards an angle of " << angle << " degrees.\n"
            << "Expected: " << glm::to_string(expectedVel) << '\n'
            << "Acutal: " << glm::to_string(pObj.GetVelocity()) << std::endl;
    }
}
