# File Name: Force_Data_Processing
# Author: Alec Trela
# Date: 2/2/23

import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt

# todo ================================ Check Ports Function ==========================================

# Source for the base code of this function: https://stackoverflow.com/questions/35724405/pyserial-get-the-name-of-the-device-behind-a-com-port


def ArduinoPort():
    """!
    Check all the ports, sees which one is connected to the Arduino and 
    returns it. 

    """

    ports = list(serial.tools.list_ports.comports())

    resultPorts = []
    descriptions = []
    for port in ports:

        # used for mac...naming convention is different
        if "n/a" not in port:
            return str(port).split(" ")[0]

        if not port.description.startswith("Arduino"):
            # correct for the somewhat questionable design choice for the USB
            # description of the Arduino Uno
            if port.manufacturer is not None:
                if port.manufacturer.startswith("Arduino") and \
                   port.device.endswith(port.description):
                    port.description = "Arduino Uno"
                else:
                    continue
            else:
                continue
        if port.device:
            resultPorts.append(port.device)
            descriptions.append(str(port.description))



    for i in descriptions:
        if "Arduino" in i:
            return port[descriptions.index(i)]

# todo ==============================================================================================


def perform_testing():
    """ 
    1. Prompt the user to enter the file name for the testing circuit
    2. Set up the communciation with the arduino serial port
        - Match the baud rate of the arduino
        - Create a serial connection
        - Tell user connection was a success
    3. Create a csv to write data to me
    4. Init loop vars
        - still_testing figures out if the arduino is still reading values
    5. Enter reading data loop
    6. Decode the message being sent from the Arduino & strip it from newline scripts
    7. Save time, force to plot later
    8. Write the data to the csv
    9. If the testing is complete, close serial connection & csv
    10. Cut off final zeroed out reading
    11. Obtain max force reading & time to obtain that value
    12. Plot 

    """

    # 1
    iteration = input("Enter file name: ")
    data_file_name = iteration + ".csv"

    # 2s
    arduino_port = ArduinoPort()
    baud = 9600
    ser = serial.Serial(arduino_port, baud)
    print("Connected to arduino port: " + arduino_port)

    # 3
    data_file = open(data_file_name, "w")
    data_file.write("[Time (s), Force (N)]\n")
    print(f"\nData File {data_file_name} generated...begin testing...")

    # 4
    still_testing = True
    time = []
    force = []

    # 5
    while True:
        line = ser.readline()
        if line and still_testing:
            # 6
            result = line.decode()
            result = result.strip()
            # 7
            time.append(float(result.split(",")[0]))
            force.append(float(result.split(",")[1]))
            # 8
            data_file.write(str(result.split(",")[0:2]) + "\n")
            # 9
            still_testing = bool(int(result[-1]))
            if not still_testing:
                print(
                    f"\nTesting complete...see {data_file_name} for results.")
                data_file.close()
                ser.close()
                break
    # 10
    time = time[0:-1]
    force = force[0: -1]
    # 11
    max_force = [max(force)]
    max_force_time = [time[force.index(max_force[0])]]
    time2max = time[0:force.index(max_force[0])+1]
    # 12
    plt.figure()
    plt.title(f"Gripper Force: {iteration}")
    plt.xlabel("Time (s)")
    plt.ylabel("Force (N)")
    plt.grid()
    plt.plot(time, force)
    plt.plot(time2max, max_force * len(time2max))
    plt.plot(max_force_time, max_force, marker="o", markersize=5,
             markerfacecolor="red")
    plt.ylim(0, max_force[0] * 1.1)
    plt.annotate("Time to Maximum: " + str(time2max[-1]) + " s" + "\nMaximum: " + str(max_force[0]) + " N",
                 xy=(time2max[len(time2max)//2], max_force[0] * 1.01))
    plt.savefig(f"{iteration}_plot.jpeg")
    plt.show()


if __name__ == "__main__":
    perform_testing()
