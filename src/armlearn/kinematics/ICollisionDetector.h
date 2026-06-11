#ifndef I_COLLISION_DETECTOR_H
#define I_COLLISION_DETECTOR_H

#include <vector>
#include <cstdint>

/**
 * @brief Interface for detecting whether a given motor configuration puts the
 * arm in collision with its own base or the floor.
 *
 * Extracted from ArmLearnWrapper::motorCollision / hasCollision.
 * Keeping it as a separate interface lets unit tests inject a stub and lets
 * the real WidowX implementation live independently of the environment logic.
 */
class ICollisionDetector {
public:
    virtual ~ICollisionDetector() = default;

    /**
     * @brief Returns true if the arm configuration described by motorPos
     * causes a geometric collision (self-collision with base, z < 0, etc.).
     *
     * @param motorPos  Six servo values in [0, 4096].
     */
    virtual bool hasCollision(const std::vector<uint16_t>& motorPos) const = 0;
};

#endif // I_COLLISION_DETECTOR_H
