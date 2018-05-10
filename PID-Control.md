
---

**PID Control Project**

The goal of this project are the following:

Implement the PID controller and control the simulator car such that the car drives through the track without leaving it.

[//]: # (Image References)
[image1]: ./examples/first.png
[image2]: ./examples/second.png
[image3]: ./examples/third.png
[image4]: ./examples/fourth.png
[image5]: ./examples/fifth.png
[video1]: ,/examples/pid.mp4

## [Rubric Points](https://review.udacity.com/#!/rubrics/824/view)
All the code for this project has been derived from the example code in the course and is in this directory.
[Here](https://github.com/gvp-study/CarND-PID-Control-Project.git)
---

### PID Controller
## Implementing the PID Controller

I implemented the PID control based on the lessons. I tuned the PID parameters manually. I used two sets of PID controllers. One for the steering and one for the throttle.

The steering PID controller was tuned by manually twiddling from a starting set of parameters. I ended up finding the PID values to be KP = 0.13, KI = 0.0005 and KD = 4.0.

The throttle PID controller was also tuned manually and I settled on KP = 0.3, KI = 0.0, KD = 0.02. I also slowed down the car based on the steering angle.

throttle = max_speed - pidt.TotalError() - max_speed*(1.0 - cos(angle*M_PI/180.0));

The result is shown in the output of the simulator at different points along the track as shown in the figures below.

![alt text][image1]

![alt text][image2]

![alt text][image3]

![alt text][image4]

The final image shows the car reached a maximum speed of 77 mph.

![alt text][image5]

The movie of the simulator controlled by the PID controller is shown below.[link to my video](./examples/pid.mp4)
![alt text][video1]
