# OpenCV Vision
A vision system for Recycle Rush 2015    
Written by Tyler Packard with help from Ben Cohen-Wang

## Overview
This program identifies yellow totes during autonomous mode and calculates the distance and angle to the tote. It works by shining a bright green light at the retro-reflective tape on the yellow totes and capturing the reflected in a camera mounted on the robot. The camera takes low exposure video and darkens the images before sending them to this program. Once this program receives the images, it masks out the retro-reflective based on predetermined hue, saturation, and value (HSV) ranges. Then all of the masked blobs are converted to polygons and non-reflective strip shaped polygons are removed.

## Calculations
A brief description of the calculations used by the program to find the distance and angle to the totes.

### Distance
The distance to yellow totes is calculated based off of the ratio of the width of the reflective strips to the width of the camera's field of view. First, the width of the camera's FOV in feet is found since we know the width of the reflective strips in pixels and feet and we know the width of the FOV in pixels too. Then the distance is found using trignometry since we know the angle of the camera's field of view.

### Angle
First, the expected width in pixels of the reflective strips at 0 degrees is found by calculating the height of the strips, since the height and width are the same in real life. Then, the actual width of the strips in pixels is found. The angle of the robot to the tote is then the arccosine of the actual width divided by the width at 0 degrees.

This formula was derived by Ben Cohen-Wang.

## Usage
The values found by this program will be went by the raspberry pi to the RoboRio. They will be used by the autonomous controller to direct the robot to the yellow totes. This allows us to create tote sets and stacked tote sets. Tote sets exist if all three yellow totes are in the auto zone, but are not stacked, and it awards the alliance 6 points. Stacked tote sets exist if all three yellow totes are in the auto zone and stacked in a legal formation, awarding the alliance 20 points.

## Problems
The angle finding formula does not return accurate angles when the robot is close to the totes (roughly less than 7.5 feet) and at a shallow angle to the tote (roughly less than 15 degrees). This happens because the the cosines of smaller angles are closer together than the cosines of larger angles. This means that the arccosine function requires increasingly accurate measurments the closer the angle is to 0 degrees, and our camera cannot provide such accurate measurements.

Another problem with this code is that it becomes very difficult to detect the retro-reflective strips from over 15 feet away. This happens because the resolution of our camera is too low, and the reflective strips are too small in the image to detect reliably.
