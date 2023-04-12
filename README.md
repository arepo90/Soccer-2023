
# Soccer 2023

Creation date: Feb 8
First stable version: Mar 2
Final version: TBD

# Documentation
The following is a (semi) thorough explanation of the logic and reasoning behind the code in this repository, made for the category of **Robocup Junior, Soccer Lightweight**. 
## OOP
All sensors and devices are referenced as objects (classes) with methods and properties related to their functions. Information is usually kept private, with the occasional getter and setter methods, since the objective is to always have up-to-date information from the return values of measurement functions.

Sensors may have different ways to be read or types of data that they return, therefore, these are chosen through the parameters of the main **read()** methods.

All objects are initialised through their constructors at the start of the program, with their main data as arguments, however, certain sensors require further actions or measurements before they can be used. In general, all objects have a private **ID** to differentiate them from similar objects, as well as **debug()** methods that output all relevant information to the *Serial monitor*.
### Motors
Each motor is controlled by three pins, an **Enable (EN)** and two **Pulse width modulation (PWM)** pins. For the motor to be active, the **EN** pin must be set to a digital *HIGH*, while only one of the **PWM** pins must have a positive analog power (1 -> 255), with the other set at 0. To change the direction of turn, the **PWM** pins must be swapped. The object has 

Each motor is treated as a different object, called M1, M2, M3 and M4, starting with the bottom left, going clockwise. When each object is constructed, the corresponding pins are set as *outputs* and the private pin numbers are defined. In addition to this, a **Default Power (defPow)** is set for each motor, which will serve as the the base speed. All pins and powers can be changed in the *#define* section at the start of the program.

The main method of the motor class is **move(int Pow)**. This takes an integer as an argument (-255 -> 255), and sets the motors to move at the power given as the argument. Note that the power can be negative, in which case the method swaps the **PWM** pins to invert the direction of turn, in addition, several checks exist to ensure a power does not overflow. A positive **Pow** argument makes the motors rotate clockwise. Every call to this method automatically updates the internal **POW** of the motor for further use.

The method of **update(int Pow)**, as the name suggests, is used to update the current **POW** of the motor. It is usually used in combination with **getPow()** to gently modify the power of the motor. Note that checks are in place to ensure that the new power does not overflow.

An underused function is that of **brake(int force)**, due to its limitations while implementing it with the rest of the code. This method takes the current power, and briefly rotates the motor in the opposite direction in order to counteract inertia. It uses the **delay()** function with milliseconds, which makes it suboptimal.

**test(int Pow)** can be called to test the motor. It will move with the power indicated in the argument for 1 second, then it will change directions with the same power for another second.

### Light sensors
Each sensor block is treated as a different object, meaning that every object is in charge of two analog pins. They each output an 8-byte number (0 -> 1024) that represents the current level of luminosity being received by the photoresistor. In this case, a simple threshold is used to determine when a line is touched.

Along with the constructors, there is a special boolean argument (arg1) that is used to determine the source of the threshold limit. A *false* value will take the limits from the *#define* section at the start, while a *true* value will take them from the Arduino's EEPROM memory through the **memRead()** and **memSave()** helper functions (documented later in the text).

As mentioned before, the **setLim(int LIM_A, int LIM_B)** method is only triggered when the special argument is *false*, thus taking the limits from the *#define* section and saving the values in the EEPROM memory. Note that this will overwrite any values previously stored.

The **read()** method outputs a state depending on the detection level. If no lines are detected, it returns 0. If only the outer sensor detects a line, it returns 1. If only the inner sensor detects a line, it returns 2. Finally, if both sensors detect a line, it returns 3. 

### Ultrasonic sensors
Each sensor requires a *trigger* and *echo* pin, as well as a *timeout*. The library of "Ultrasonic.h" is used to handle measurements. When the US objects are constructed, each carries a sub-object from the library, which is then initialised using the arguments. When a measurement is made through the **read()** method, a call is made to the secondary object.

Ultrasonic sensors are highly unreliable, and thus several measures have been taken to mitigate their impact. The timeout can be modified to increase the reading distance of the sensor, although "false positives" still exist, thus an average or such function should be made to handle these cases. Currently WIP.

### Compass
The compass sensor requires a secondary initialisation after the object is constructed, which is only called when the **globalInit()** argument is 1 or 3 (documented later in the text). The primary initialisation sets the pins, addresses and limits, while the secondary starts I2C bus to start communication with the sensor.

The **init()** starts the **Wire** communication by sending a blank package to the sensor's address. After the initial handshake is performed, a measurement is made to set the *OFFSET* of the compass. This is only done at the start of the program to get the fake north we will be following.

The **read(int mode)** is the main method of this class, and it's used to measure the current angle of the compass, taking into account the initial *OFFSET*. The argument, or *mode*, determines the type of data the function will return, either as a degree (0 -> 360), or as a decimal angle (-1 -> 1). Note that degrees start at 0 (north) and increase clockwise until 360 (also north), while decimal starts at 0 (north), increases clockwise until 1 (south), and decreases counterclockwise until -1 (also south). As the name suggests, the second mode requires floating point arithmetic, therefore the entire method returns a *double* data type. To make a measurement, a request is sent through **Wire** to the sensor's address, and two bytes are received as the response. These are used to make up the angle, and the necessary operations are performed to output it in the correct format.

This class also carries secondary getters and setters for the *OFFSET*, as well as the **north()** helper method to determine if the compass is currently pointing north. Note that this is not the magnetic north, but rather the initial direction of the robot.

### Infrared Seeker

Similarly to the compass sensor, the IRSeeker requires I2C communication, and thus is initialised after its construction if the **globalInit()** arguments are 2 or 3. The primary initialisation sets the pins, addresses and limits, while the secondary starts I2C bus to start communication with the sensor.

The **init()** method starts the **Wire** communication by sending a blank package to the sensor's address and awaiting for a response, thus completing the handshake.

The **read(int mode)** is also the main method of this class, and is used to know the current sector the IR ball is in. The mode determines if the result is sent as the sector (0 -> 9), or as the angle of the sector (-120 -> 120). Note that, either way, the resolution of the result is 30 degrees, and if no ball is found, the second mode returns NaN. The information is requested by sending a message through the **Wire** to the sensor's address, and awaiting for a 6-byte response, although only the first byte represents the direction of the ball.


## Main Functions
The combination of methods and objects are used in separate functions to perform specific tasks.

### Basic movement
The four movements in the main axis are performed through specific functions, with the use of the Motor **move()** methods, **stop stp(), forward fwd(), backward bwd(), left lef(), right rig()**. These do not take arguments, but rather use the default powers of each motor, with the exception of **stp()**, which argument determines if the stop is regular, or through the use of the **brake()** method.

The main testing function is **rotate(int n)**, which simply moves all motors in the same direction with the power determined by the argument.

Additional functions can be found as **movements()** or **diagonals**, with varying testing purposes.

### Helpers
Secondary functions can be found throughout the program, with varying purposes and uses. 

**globalInit(int mode)** is called during the setup to initialise Serial communications and other functions. An argument of 0 will do only that, 1 will initialise the compass, 2 will initialise the IRSeeker, and 3 will initialise all.

**tracker()** is used at every cycle with to make an LED blink so as to detect program malfunctions, while **checkDelay(int time)** serves as a general non-interrupting delay for other functions. It returns a *true* value when the required time has been completed, and *false* otherwise.

The main debug functions, **motorDebug(), lightDebug(), usDebug(), memDebug(), globalDebug()**, are simply used to prevent repetition in calling the **debug()** method of every object, although they each require a new line in the Serial monitor.

The angle functions, **degToDec()** and **decToDeg()** are used interchangeably to modify the format of angles and measurements throughout the program. The first one changes sexagesimal angles (0 -> 360) to decimals (-1 -> 1), and the other one serves the opposite purpose.

For the EEPROM memory, the main functions used are **memSave(int n, int target)**, that saves the *n* argument to the *target* memory address, while **memRead(int target)** returns the values in the *target* memory address. Note that EEPROM works by having an array of bytes, therefore arrangements have been made so that every memory address (such as those referenced by the *target*) represents a 4-byte segment (0 -> 1024) of cells.

One of the most important helper functions is **followPath(double angle)**. This takes a decimal angle as argument and manages the motor powers to drive the robot towards that angle. Every motor has a different relevance on movements due to their directions and positioning, thus *sin* functions are used to determine the power that each motor should have to follow the path given.

### Game Plan
The main movement function of the robot is **vectorControl(double angle)**. As the name suggests, a vector is given and the robot follows it, taking into account orientation. Initially, the **followPath()** function is called to follow the given angle, afterwards, a calculation is made to determine the degree of error in the orientation of the robot thanks to compass measurements. The error is multiplied by a predetermined *FACTOR* in order to make it more precise and faster, and the powers are updated accordingly. Through testing, it has been found that a low *FACTOR* (less than 3) causes the robot to make corrections very slowly, and over a large distance, while a high *FACTOR* (over 6) makes the robot solely focus on correcting the angle, thus making it seem as if a separate function interrupted the **followPath()** workings until the correct angle was reached. Depending on the default powers of the motors, the factor should be adjusted in the 3 to 6 range.

If necessary, a dedicated **orientation()** function exists that focuses on correcting the direction of the robot. This uses a proportional change with the minimum and maximum powers, as well as threshold, determined by the *#define* section.

The **line(int mode)** is one of the main game functions. The *mode* argument determines how the reaction is managed, either as a "reflex" (simply going the opposite way when a line is touched), or as a full stop. If the second option is chosen, the robot completely stops until the ball changes directions or disappears. This is to prevent continuous "bouncing" on the lines, that may result in being out of bounds. Further testing is required, as the usage of **delay()** can be both beneficial and hurting in different scenarios.

The **ball(int mode)** is the other main game functions. The argument determines if the ball is followed through its angle with the use of **vectorControl()**, or the archaic version of separate movements for each sector the ball may be in. Further testing is required, as the position of the IRSeeker sensor may influence on the movements and reactions of the robot. It may be necessary to add specific cases to induce "diagonals", and other such movements, to maximise the ways that the robot hits the ball from the front, not the sides.

The final game function is **comeback()** which handles the robot when the ball is not in sight. Ideally, ultrasonic sensors should be used to manage movements, however, their unreliableness makes them hard to work with, and currently, the comeback is simply to go back with the **vectorControl()** function.

During game, only the **gp()** function should be called in the **void loop()** to centralise the code. This function will have all the others and serve as the main strategy.

## Author
Esteban Martinez

Arepo90
