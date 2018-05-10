#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

high_resolution_clock::time_point current_t = high_resolution_clock::now();
double max_speed = 0.7;
double odometer = 0.0;
double total_cte = 0.0;

int main(int argc, char** argv)
{
  uWS::Hub h;
//  float Kp = 0.1, Ki = 0.0003, Kd = 3.0;
//    float Kp = 0.125, Ki = 0.0003, Kd = 4.0;
//    float Kp = 0.13, Ki = 0.0003, Kd = 4.0;
    float Kp = 0.13, Ki = 0.0005, Kd = 4.0;
//    float Kpt = 0.5, Kit = 0.0, Kdt = 0.08;
//    float Kpt = 0.7, Kit = 0.0, Kdt = 0.04;
//    float Kpt = 0.5, Kit = 0.0, Kdt = 0.01;
    float Kpt = 0.3, Kit = 0.0, Kdt = 0.02;
  if(argc > 1)
    Kp = atof(argv[1]);
  if(argc > 2)
    Ki = atof(argv[2]);
  if(argc > 3)
    Kd = atof(argv[3]);
  if(argc > 4)
    max_speed = atof(argv[4]);
  PID pid;
  PID pidt;
  // TODO: Initialize the pid variable.
  pid.Init(Kp, Ki, Kd);
  pidt.Init(Kpt, Kit, Kdt);
  
  h.onMessage([&pid, &pidt](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value = 0.0, throttle = 0.3;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
	  pid.UpdateError(cte);
	  steer_value = -pid.TotalError();

	  high_resolution_clock::time_point t2 = high_resolution_clock::now();
	  duration<double> del = duration_cast<duration<double>>(t2 - current_t);
	  double dt = del.count();
	  current_t = t2;
	  odometer += speed*dt;
	  total_cte += fabs(cte);
	  pidt.UpdateError(cte);
	  throttle = max_speed - pidt.TotalError() - max_speed*(1.0 - cos(angle*M_PI/180.0));

          // DEBUG
          std::cout << "CTE: " << cte;
	  std::cout << " Steering Value: " << steer_value;
          std::cout << " Throttle Value: " << throttle;
          std::cout << " Steering Angle: " << angle;
          std::cout << " Delta T: " << dt;
          std::cout << " Odometer : " << odometer;
          std::cout << " Total Error : " << total_cte;
          std::cout << " Error / Distance : " << total_cte/odometer << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;//0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
