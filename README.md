# MotionCaptureTest
Motion capture test with OpenGL and my Android phone

I haven't seen anyone doing anything too interesting with the gyroscopes and accelerometers in their smartphones, so I decided to make a proof-of-concept using my Android smartphone's Accelerometer and Gyroscope (basically as a motion controller), and an OpenGL demo running on my laptop.

![Demo Gif](https://raw.githubusercontent.com/ShrinathN/MotionCaptureTest/master/img/Test.gif)

The results were great, I was able to rotate the cube using my smartphone. There was some input lag, but that was solved by increasing the sampling rate on my smartphone, and switching from my normal WiFi network to using my phone as a WiFi hotspot.

![App Working](https://raw.githubusercontent.com/ShrinathN/MotionCaptureTest/master/img/App.gif)

I also wrote a python script (uses pandas and matplotlib) to record and plot the gyroscope and accelerometer data, for any future use, analysis etc.

![Graph by the python script](https://raw.githubusercontent.com/ShrinathN/MotionCaptureTest/master/img/Figure_1.png)

I may make this into a game or something. Poor man's VR motion sensors