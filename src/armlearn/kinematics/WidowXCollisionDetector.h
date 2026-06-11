#ifndef WIDOWX_COLLISION_DETECTOR_H
#define WIDOWX_COLLISION_DETECTOR_H

#include "ICollisionDetector.h"
#include <cmath>
#include <algorithm>

/**
 * @brief Concrete collision detector for the WidowX robotic arm.
 *
 * All geometry previously embedded in ArmLearnWrapper::motorCollision() and
 * ArmLearnWrapper::hasCollision() lives here. The class is stateless and
 * therefore trivially testable and thread-safe.
 *
 * Known limitations (inherited from original code, documented here for
 * future work):
 *  - angle_base is computed but NOT used in the projection: the model is
 *    effectively 2D (sagittal plane only). A 3D base rotation is needed to
 *    detect collisions when the arm points backward toward its own base.
 *  - hasCollisionBetweenSegments() only handles axis-aligned base segments.
 *    A diagonal base segment would trigger a spurious "true" via the
 *    fall-through return.
 *  - Floating-point equality comparisons (==) should use an epsilon.
 */
class WidowXCollisionDetector : public ICollisionDetector {
public:

    // -----------------------------------------------------------------------
    // Physical constants (mm) — named so they can be updated in one place.
    // -----------------------------------------------------------------------
    static constexpr uint16_t LENGTH_BASE     = 125;
    static constexpr uint16_t LENGTH_SHOULDER = 142;
    static constexpr uint16_t LENGTH_ELBOW    = 142;
    static constexpr uint16_t LENGTH_WRIST    = 155;
    static constexpr uint16_t DISPLACEMENT    = 49;

    // Base obstacle segments: {xA, xB, yA, yB}
    const std::vector<std::vector<double>> baseSegments = {
        { 68,  90,   9,   9}, { 68,  68,   9,  95}, { 15,  68,  95,  95}, { 15,  15,  95, 151},
        {-68, -90,   9,   9}, {-68, -68,   9,  95}, {-15, -68,  95,  95}, {-15, -15,  95, 151},
    };

    // -----------------------------------------------------------------------
    // ICollisionDetector
    // -----------------------------------------------------------------------
    bool hasCollision(const std::vector<uint16_t>& motorPos) const override {
        return motorCollision(motorPos);
    }

private:

    // -----------------------------------------------------------------------
    // Geometry
    // -----------------------------------------------------------------------
    bool motorCollision(const std::vector<uint16_t>& newMotorPos) const {

        // Angle conversions
        // NOTE: angle_base / radiant_angle_base are computed but not yet used
        // in the 2D projection. Kept for documentation and future 3D fix.
        const double angle_base     = static_cast<double>(newMotorPos[0]) / 4096.0 * 360.0;
        const double angle_shoulder = (static_cast<double>(newMotorPos[1]) - 1024.0) / 2048.0 * 180.0;
        const double angle_elbow    = (static_cast<double>(newMotorPos[2]) - 1024.0) / 2048.0 * 180.0;
        const double angle_wrist    = (static_cast<double>(newMotorPos[3]) - 1024.0) / 2048.0 * 180.0;

        const double rad_base     = angle_base     / 180.0 * M_PI; // TODO: use in 3D extension
        const double rad_shoulder = angle_shoulder / 180.0 * M_PI;
        const double rad_elbow    = M_PI - angle_elbow / 180.0 * M_PI;
        const double rad_wrist    = M_PI / 2.0 - angle_wrist / 180.0 * M_PI;

        // Forward kinematics (2D sagittal plane)
        const double val1_side = std::cos(rad_shoulder) * LENGTH_SHOULDER;
        const double val2_side = val1_side + std::cos(rad_shoulder + M_PI / 2.0) * DISPLACEMENT;
        const double val3_side = val2_side + std::cos(rad_shoulder + rad_elbow) * LENGTH_ELBOW;
        // val4_side intentionally unused below; kept for future wrist segment

        const double val1_z = std::sin(rad_shoulder) * LENGTH_SHOULDER + LENGTH_BASE;
        const double val2_z = val1_z + std::sin(rad_shoulder + M_PI / 2.0) * DISPLACEMENT;
        const double val3_z = val2_z + std::sin(rad_shoulder + rad_elbow) * LENGTH_ELBOW;
        const double val4_z = val3_z + std::sin(rad_shoulder + rad_elbow + rad_wrist) * LENGTH_WRIST;

        // Ground-plane check (z < 0 means below the table)
        if (val1_z < 0.0 || val2_z < 0.0 || val3_z < 0.0 || val4_z < 0.0) {
            return true;
        }

        // Wrist / gripper bounding box segments
        const double angle_wep = rad_shoulder + rad_elbow + rad_wrist; // wrist-end-point angle

        const double cx1 = val3_side + std::cos(angle_wep) * 160.0 + std::cos(angle_wep - M_PI/2.0) *  24.0;
        const double cx2 = val3_side + std::cos(angle_wep) * 160.0 + std::cos(angle_wep - M_PI/2.0) * -32.0;
        const double cx3 = val3_side +                                std::cos(angle_wep - M_PI/2.0) * -32.0;

        const double cy1 = val3_z + std::sin(angle_wep) * 160.0 + std::sin(angle_wep - M_PI/2.0) *  24.0;
        const double cy2 = val3_z + std::sin(angle_wep) * 160.0 + std::sin(angle_wep - M_PI/2.0) * -32.0;
        const double cy3 = val3_z +                                std::sin(angle_wep - M_PI/2.0) * -32.0;

        // Additional ground check for gripper tips
        if (cy1 < 0.0 || cy2 < 0.0) {
            return true;
        }

        // Elbow segment
        const double angle_ep = rad_shoulder + rad_elbow;

        const double cx4 = val3_side + std::cos(angle_ep) * 10.0 + std::cos(angle_ep - M_PI/2.0) *  18.0;
        const double cx5 = val3_side + std::cos(angle_ep) * 10.0 + std::cos(angle_ep - M_PI/2.0) * -18.0;
        const double cx6 = val2_side +                              std::cos(angle_ep - M_PI/2.0) * -18.0;

        const double cy4 = val3_z + std::sin(angle_ep) * 10.0 + std::sin(angle_ep - M_PI/2.0) *  18.0;
        const double cy5 = val3_z + std::sin(angle_ep) * 10.0 + std::sin(angle_ep - M_PI/2.0) * -18.0;
        const double cy6 = val2_z +                              std::sin(angle_ep - M_PI/2.0) * -18.0;

        const std::vector<std::vector<double>> armSegments = {
            {cx1, cx2, cy1, cy2},
            {cx3, cx2, cy3, cy2},
            {cx4, cx5, cy4, cy5},
            {cx6, cx5, cy6, cy5},
        };

        for (const auto& armSeg : armSegments) {
            for (const auto& baseSeg : baseSegments) {
                if (hasCollisionBetweenSegments(armSeg, baseSeg)) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * @brief 2D segment–segment intersection test.
     *
     * Precondition: baseSegment must be either horizontal (yA == yB) or
     * vertical (xA == xB). Diagonal base segments produce a false positive
     * (fall-through returns true) — this is a known limitation.
     *
     * @param armSegment   {xA, xB, yA, yB}
     * @param baseSegment  {xA, xB, yA, yB}  — axis-aligned obstacle edge
     */
    bool hasCollisionBetweenSegments(
        const std::vector<double>& armSegment,
        const std::vector<double>& baseSegment) const
    {
        const bool isHorizontal = (baseSegment[2] == baseSegment[3]);
        const bool isVertical   = (baseSegment[0] == baseSegment[1]);

        if (isHorizontal) {
            // Quick AABB reject
            if (std::min(armSegment[0], armSegment[1]) > std::max(baseSegment[0], baseSegment[1])) return false;
            if (std::max(armSegment[0], armSegment[1]) < std::min(baseSegment[0], baseSegment[1])) return false;
            if (std::max(armSegment[2], armSegment[3]) < baseSegment[2]) return false;
            if (std::min(armSegment[2], armSegment[3]) > baseSegment[2]) return false;

            // Arm segment is vertical: guaranteed to cross the horizontal line
            if (armSegment[0] == armSegment[1]) return true;

            // Line equation y = a*x + b; find x where y == baseSegment[2]
            const double a    = (armSegment[3] - armSegment[2]) / (armSegment[1] - armSegment[0]);
            const double b    = armSegment[2] - a * armSegment[0];
            const double xInt = (baseSegment[2] - b) / a;

            if (xInt > std::max(baseSegment[0], baseSegment[1])) return false;
            if (xInt < std::min(baseSegment[0], baseSegment[1])) return false;

        } else if (isVertical) {
            // Quick AABB reject
            if (std::min(armSegment[2], armSegment[3]) > std::max(baseSegment[2], baseSegment[3])) return false;
            if (std::max(armSegment[2], armSegment[3]) < std::min(baseSegment[2], baseSegment[3])) return false;
            if (std::max(armSegment[0], armSegment[1]) < baseSegment[0]) return false;
            if (std::min(armSegment[0], armSegment[1]) > baseSegment[0]) return false;

            if (armSegment[0] == armSegment[1]) {
                return (armSegment[1] == baseSegment[1]);
            }

            // Line equation y = a*x + b; find y where x == baseSegment[0]
            const double a    = (armSegment[3] - armSegment[2]) / (armSegment[1] - armSegment[0]);
            const double b    = armSegment[2] - a * armSegment[0];
            const double yInt = a * baseSegment[0] + b;

            if (yInt > std::max(baseSegment[2], baseSegment[3])) return false;
            if (yInt < std::min(baseSegment[2], baseSegment[3])) return false;

        } else {
            // Diagonal base segment — not expected in current configuration.
            // Returning true (conservative: treat as collision) to match
            // original behaviour and surface the problem early.
            return true;
        }

        return true;
    }
};

#endif // WIDOWX_COLLISION_DETECTOR_H
