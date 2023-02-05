# Gripper-Unit-Testing

## File Utilities

### Force_Data_Processing.py
Reads force data sent over serial from Arduino, and generates a .csv and jpeg of the trial run. 
<br>
![force-reading](https://github.com/artrela/Gripper-Unit-Testing/blob/main/example_plot.jpeg)
*Fig. Example force reading generated from Force_Data_Processing.py*
<br>
### Force_Testing.ino
Reads force data from a Force Resistive Sensor (FSR) for a specific trial run of the end-effector. 

### servo.ino
Runs a servo motor through one set of opening and closing, to pair with the data reading procedure.
