#pragma once

#include "src/hal/driver/Driver.hpp"

/**
 * Rotation axis used for most movements of the tracking mount. The axis class calculates
 * conversions from arcsecs to steps of its stepper taking into account transmission,
 * max speed, stepper and driver characteristics etc. It should be used by the mount without
 * exposing detailed information about the used hardware.
 **/
class RotationAxis
{

public:
    /**
     * Perform initial setup of this axis. This function will setup all the hardware used for it.
     */
    virtual void setup();

    /**
     * Perform required calculations and make a motor step if needed.
     * This function has to be called periodically as fast as possible and at least 1 time per step.
     */
    virtual void loop();

protected:
    /**
     * Construct a new RotationAxis instance.
     * 
     * @param transmission  mTransmission value of this axis.
     *                      If e.g. RA ring circumference is 100mm and used pulley is 20mm,
     *                      resulting mTransmission would be (circ_ra / circ_pulley) = (100 / 20) = 5
     * @param driver        reference to a specific implementation instance of the Driver interface.
     */
    RotationAxis(float transmission, Driver* driver);

    /**
     * Get current axis position in degrees.
     */
    float getCurrentDegrees() const;

    /**
     * Set current axis position.
     *
     * @param degrees Position to be set in degrees. Negative values are allowed.
     */
    void setCurrentPosition(float degrees);

    /**
     * Rotate the axis at the specified speed.
     * 
     * @param degPerSecond Rotation speed. Negative for reversed direction. Zero for stop.
     */
    void setSpeed(float degPerSecond);

    /**
     * Rotate the axis at the specified speed to a target and stop after this target was reached. The last loop() call 
     * will also call onTargetReached() so the subclass can react.
     * 
     * @param degrees target of rotation in deg (absolute value)
     */
    void moveTo(float degrees);

    /**
     * Rotate the axis at the specified speed to a target and stop after this target was reached. The last loop() call 
     * will also call onTargetReached() so the subclass can react.
     * 
     * @param degrees target of rotation in deg (relative value)
     */
    void moveBy(float degrees);

    /**
     * Callback to be called after the rotation target was reached. This function will be called at the end of the 
     * loop() call which leads to reaching the target set in rotateToTarget().
     */
    virtual void onTargetReached();

private:

    /**
     * Current mPosition of this axis in degrees.
     */
    float mPositionDeg = 0.0f;

    /**
     * Transmission ratio of this axis
     */
    const float mTransmission;

    Driver* mDriver;

    /**
     * Distance in degrees to the target. 0 if there is no target set or last target was reached.
     */
    float mDegsToTarget = 0.0f;
};
