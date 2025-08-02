#include "UAV.hpp"
#include <fstream>
#include <iomanip>
#include <cmath>

UAV::UAV(int id_, double x0, double y0, double z0,
         double v0, double az0, double turnR)
  : id(id_), x(x0), y(y0), z(z0), v(v0), az(az0), turnRadius(turnR),
    target(x0,y0)
{}

void UAV::setTarget(double tx, double ty) { 
    target = {tx, ty}; 
    mode = Mode::FLYING;
}

void UAV::update(double dt, double currentTime) { 
    double dx = target.first  - x;
    double dy = target.second - y;
    double distanceToTarget = std::sqrt(dx*dx + dy*dy);

    const double distRadius = 0.5; // distance border to consider the UAV at the target

    if (mode == Mode::FLYING) {
        //calculate angle to target
        // atan2 returns the angle in radians, we convert it to degrees
        double angleToTarget = std::atan2(dy, dx);

        double targetAz;
        // Calculate the target azimuth based on the distance to the target and the turn radius
        if (distanceToTarget > turnRadius) {
            // If the UAV is far enough from the target
            
            //calc the angle of intersection between the UAV's path and the tangent to the circle 
            //acos returns the angle in radians
            double intersectionAngle  = std::acos(turnRadius / distanceToTarget);

            // update the target azimuth to be the angle to the target plus the intersection angle 
            // We add M_PI/2 to get the azimuth in the correct direction- meaning tangent to the circle
            double targetAzRad = angleToTarget + (M_PI/2 - intersectionAngle);

            // convert the target azimuth to degrees and ensure it's in the range [0, 360)
            // fmod is used to ensure the azimuth is within the range [0, 360)
            targetAz = std::fmod(targetAzRad * 180.0 / M_PI + 360.0, 360.0);
        } else {
            // If the UAV is within the turn radius, calculate the azimuth based on the angle to the target
            //there's no need to calculate the intersection angle, just use the angle to the target
            //we are under the assupmtion that hard turn are a special case- in which the UAV must turn to the target directly
            double targetAzRad = angleToTarget + M_PI/2;
            targetAz = std::fmod(targetAzRad * 180.0 / M_PI + 360.0, 360.0);
        }   
        // Calculate the difference between the current azimuth and the target azimuth
        //take the shortest path to the target azimuth 
        double deltaAz = targetAz - az;
        if (deltaAz > 180){
            deltaAz -= 360;
        } 
        if (deltaAz < -180) {
            deltaAz += 360;
        }

        // Calculate the maximum turn angle based on velocity, dt and turn radius
        double max_turn_degree = (v / turnRadius) * 180.0 / M_PI * dt; 
        // adjust the azimuth towards the target azimuth
        if (std::abs(deltaAz) <= max_turn_degree) {
            az = targetAz;
        } else {
                if (deltaAz > 0) {
                    az += max_turn_degree;  // turn right
                }else {
                    az -= max_turn_degree;  // turn left
                }
        }

        az = std::fmod(az + 360.0, 360.0); // ensure total azimuth is in the range [0, 360)

        // Update the position based on the current azimuth and velocity
        double radianval = az * M_PI / 180.0; // convert azimuth to radians for sin and cos
        // Update the position based on the current azimuth and velocity by the ceequation for angular distan: dist = velocity * time * angle
        x += v * dt * std::cos(radianval); 
        y += v * dt * std::sin(radianval);

        // Check if the UAV is close enough to the target to start orbiting
        // If the UAV is within the turn radius of the target, switch to await(orbit) mode

        //re calculate the distance to the target- since the position has been updated
        dx = target.first  - x;
        dy = target.second - y; 
        distanceToTarget = std::sqrt(dx*dx + dy*dy);

        // If the UAV is within the turn radius of the target, switch to awaiting mode
        // We add the distRadius to ensure the UAV is close enough to the target to start orbiting
        if (distanceToTarget <= turnRadius + distRadius) {
            // calculate the angle to the target, atan2 returns the angle in radians
            loopPos = std::atan2(y - target.second, x - target.first);
            
            // placing the UAV on the orbiting path
            //using the formula for a circle: x = center_x + radius * cos(angle), y = center_y + radius * sin(angle)
            x = target.first  + turnRadius * std::cos(loopPos); // x coordinate on the orbiting path
            y = target.second + turnRadius * std::sin(loopPos); // y coordinate on the orbiting path

            // Update the azimuth to point towards the target position to be tangent to the circle
            az = std::fmod((loopPos - M_PI/2) * 180.0/M_PI + 360.0, 360.0);
            // Set the mode to awaiting (orbiting) mode
            mode = Mode::AWAITING;
            return; // Exit the update function to avoid further position updates in flying mode
        }

    
    }
    else if (mode == Mode::AWAITING){
        double angledSpeed = v / turnRadius; // calculate the angular speed based on velocity and turn radius in [rad/s]
        loopPos -= angledSpeed * dt; // calculate the new angle on the orbiting path - we subtract because we want to orbit counter-clockwise
        
        // Update the position based on the orbiting angle:
        //x = target.first  + turnRadius * std::cos(loopPos)
        //y = target.second + turnRadius * std::sin(loopPos)
        
        x = target.first  + turnRadius * std::cos(loopPos);
        y = target.second + turnRadius * std::sin(loopPos);

        // Update the azimuth to point towards the target position 
        // azimuth is the angle of the tangent to the circle at the current position
        // azimuth = loopPos + 90 degrees (or M_PI/2 radians)

       double tangAz = loopPos - M_PI/2;   //  tangent angle in radians
        az = std::fmod(tangAz * 180.0 / M_PI + 360.0, 360.0); // // convert to degrees and ensure it's in the range [0, 360)

    }
    // Save the current state to history
    history.emplace_back(currentTime, x, y, az); 
}

void UAV::saveHistory(const std::string& folder) const {
    
    std::ofstream f(folder + "/UAV" + std::to_string(id) + ".txt");
    f << "time x y azimuth\n";

 for (const auto& entry : history) { //for each entry in the history 
    //extract the values from the entry
        double t  = std::get<0>(entry);
        double xx = std::get<1>(entry);
        double yy = std::get<2>(entry);
        double aa = std::get<3>(entry);
        
             //write the entry to the file
        f << std::fixed << std::setprecision(2) //fixed- displays numbers as decimals, setprecision- determins to what extent beyond "."
          << t << " " << xx << " " << yy << " " << aa << "\n"; 
    }
}
