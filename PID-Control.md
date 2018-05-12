
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

## Steering PID
The steering PID controller was tuned by manually twiddling from a starting set of parameters. I ended up finding the PID values to be Kp = 0.13, Ki = 0.0001 and Kd = 4.0.

## Reflections on the Steering PID terms
To find the appropriate values of the PID terms I manually twiddled the parameters. For this I needed to find an error measure which could incorporate the error for the whole track and normalize it by the distance. So I integrated the distance from the increments and the absolute CTE and obtained the ratio of the latter to the former. I then twiddled the PID terms based on this value after one track run.
```code
odometer += speed*dt;
total_cte += fabs(cte);
std::cout << " Error / Distance : " << total_cte/odometer << std::endl;
```
### Propotional Term P
I started out with just the P term and observed that the system oscillates when only the P term is nonzero as explained in the course notes. I settled on the value of 0.13.
### Differential Term D
I then increased the differential term from 0, 1, 2, 3, till 4.0 when I found it could keep the car on track for a whole lap.
### Integral Term I
I found that even low values of I brought back oscillations in the driving. So I manually twiddled it to 0.0001. This small value reduced the total_cte/odometer error from experiments.

## Throttle PID
The throttle PID controller was also tuned manually and I settled on Kp = 0.3, Ki = 0.0, Kd = 0.02. In addition to the PID term from the CTE, I also slowed down the car based on the steering angle. This allows the car to slow down at all turns.
```code
throttle = max_speed - pidt.TotalError() - max_speed*(1.0 - cos(steering_angle*M_PI/180.0));
```

The result is shown in the output of the simulator at different points along the track as shown in the figures below.

![alt text][image1]

![alt text][image2]

![alt text][image3]

![alt text][image4]

The final image shows the car reached a maximum speed of 77 mph.

![alt text][image5]

The movie of the simulator controlled by the PID controller is shown below.[link to my video](./examples/pid.mp4)
![alt text][video1]
