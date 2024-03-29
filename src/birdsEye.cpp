//
// Created by asif on 16.09.19.
//

#include "birdsEye.h"

cv::Mat birdsEye::birdsEyeFunction(cv::Mat plotted_image) {

  int alpha_ = 20, beta_ = 90, gamma_ = 90;
  int f_x = m_camera_fx, dist_ = 500;
  int f_y = m_camera_fy;

  //   cv::namedWindow("BirdsEye", 1);

  //  cv::createTrackbar("Alpha", "BirdsEye", &alpha_, 180);
  /*  cv::createTrackbar("Beta", "BirdsEye", &beta_, 180);
    cv::createTrackbar("Gamma", "Result", &gamma_, 180);
    cv::createTrackbar("f_x", "Result", &f_x, 2000);
    cv::createTrackbar("f_y","Result", &f_y, 2000);
    cv::createTrackbar("Distance", "Result", &dist_, 2000);
*/

  double focalLength_x, focalLength_y, dist, alpha, beta, gamma;

  alpha = ((double)alpha_ - 90) * PI / 180;
  beta = ((double)beta_ - 90) * PI / 180;
  gamma = ((double)gamma_ - 90) * PI / 180;
  focalLength_x = (double)f_x;
  focalLength_y = (double)f_y;
  dist = (double)dist_;

  cv::Size image_size = plotted_image.size();

  double w = (double)image_size.width, h = (double)image_size.height;

  cv::Mat A1 =
      (cv::Mat_<float>(4, 3) << 1, 0, -w / 2, 0, 1, -h / 2, 0, 0, 0, 0, 0, 1);

  cv::Mat RX = (cv::Mat_<float>(4, 4) << 1, 0, 0, 0, 0, cos(alpha), -sin(alpha),
                0, 0, sin(alpha), cos(alpha), 0, 0, 0, 0, 1);

  cv::Mat RY = (cv::Mat_<float>(4, 4) << cos(beta), 0, -sin(beta), 0, 0, 1, 0,
                0, sin(beta), 0, cos(beta), 0, 0, 0, 0, 1);

  cv::Mat RZ = (cv::Mat_<float>(4, 4) << cos(gamma), -sin(gamma), 0, 0,
                sin(gamma), cos(gamma), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

  // R - rotation matrix
  cv::Mat R = RX * RY * RZ;

  // T - translation matrix
  cv::Mat T = (cv::Mat_<float>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, dist,
               0, 0, 0, 1);

  // K - intrinsic matrix
  cv::Mat K = (cv::Mat_<float>(3, 4) << focalLength_x, 0, w / 2, 0, 0,
               focalLength_y, h / 2, 0, 0, 0, 1, 0);

  cv::Mat transformationMat = K * (T * (R * A1));

  char str[200];

  sprintf(str, "WARPED IMAGE, ALPHA = 25");

  warpPerspective(plotted_image, _destination, transformationMat, image_size,
                  cv::INTER_CUBIC | cv::WARP_INVERSE_MAP);

  putText(_destination, str, cv::Point2f(100, 100), cv::FONT_HERSHEY_PLAIN, 2,
          cv::Scalar(0, 255, 0, 255));

  /*
      imshow("BirdsEye", _destination);

      cv::waitKey(100);
  */

  return _destination;
}

double
birdsEye::scalingFactor(std::vector<std::vector<cv::Point2f>> &aruco_corners,
                        sl::Resolution aruco_resolution) {}

cv::Mat birdsEye::deNoise(cv::Mat inputImage) {
  cv::Mat output;

  cv::GaussianBlur(inputImage, output, cv::Size(3, 3), 0, 0);

  return output;
}

cv::Mat birdsEye::edgeDetector(cv::Mat img_noise) {
  cv::Mat output;
  cv::Mat kernel;
  cv::Point anchor;

  // Convert image from RGB to gray
  cv::cvtColor(img_noise, output, cv::COLOR_RGB2GRAY);
  // Binarize gray image
  cv::threshold(output, output, 140, 255, cv::THRESH_BINARY);

  // Create the kernel [-1 0 1]
  // This kernel is based on the one found in the
  // Lane Departure Warning System by Mathworks
  anchor = cv::Point(-1, -1);
  kernel = cv::Mat(1, 3, CV_32F);
  kernel.at<float>(0, 0) = -1;
  kernel.at<float>(0, 1) = 0;
  kernel.at<float>(0, 2) = 1;

  // Filter the binary image to obtain the edges
  cv::filter2D(output, output, -1, kernel, anchor, 0, cv::BORDER_DEFAULT);

  return output;
}

cv::Mat birdsEye::mask(cv::Mat img_edges) {
  cv::Mat output;
  cv::Mat mask = cv::Mat::zeros(img_edges.size(), img_edges.type());
  cv::Point pts[4] = {cv::Point(210, 720), cv::Point(550, 450),
                      cv::Point(717, 450), cv::Point(1280, 720)};

  // Create a binary polygon mask
  cv::fillConvexPoly(mask, pts, 4, cv::Scalar(255, 0, 0));
  // Multiply the edges image and the mask to get the output
  cv::bitwise_and(img_edges, mask, output);

  return output;
}

// HOUGH LINES

std::vector<cv::Vec4i> birdsEye::houghLines(cv::Mat img_mask) {
  std::vector<cv::Vec4i> line;

  // rho and theta are selected by trial and error
  HoughLinesP(img_mask, line, 1, CV_PI / 180, 20, 20, 30);

  return line;
}

std::vector<std::vector<cv::Vec4i>>
birdsEye::lineSeparation(std::vector<cv::Vec4i> lines, cv::Mat img_edges) {
  std::vector<std::vector<cv::Vec4i>> output(2);
  size_t j = 0;
  cv::Point ini;
  cv::Point fini;
  double slope_thresh = 0.3;
  std::vector<double> slopes;
  std::vector<cv::Vec4i> selected_lines;
  std::vector<cv::Vec4i> right_lines, left_lines;

  // Calculate the slope of all the detected lines
  for (auto i : lines) {
    ini = cv::Point(i[0], i[1]);
    fini = cv::Point(i[2], i[3]);

    // Basic algebra: slope = (y1 - y0)/(x1 - x0)
    double slope =
        (static_cast<double>(fini.y) - static_cast<double>(ini.y)) /
        (static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);

    // If the slope is too horizontal, discard the line
    // If not, save them  and their respective slope
    if (std::abs(slope) > slope_thresh) {
      slopes.push_back(slope);
      selected_lines.push_back(i);
    }
  }

  // Split the lines into right and left lines
  img_center = static_cast<double>((img_edges.cols / 2));
  while (j < selected_lines.size()) {
    ini = cv::Point(selected_lines[j][0], selected_lines[j][1]);
    fini = cv::Point(selected_lines[j][2], selected_lines[j][3]);

    // Condition to classify line as left side or right side
    if (slopes[j] > 0 && fini.x > img_center && ini.x > img_center) {
      right_lines.push_back(selected_lines[j]);
      right_flag = true;
    } else if (slopes[j] < 0 && fini.x < img_center && ini.x < img_center) {
      left_lines.push_back(selected_lines[j]);
      left_flag = true;
    }
    j++;
  }

  output[0] = right_lines;
  output[1] = left_lines;

  return output;
}

// REGRESSION FOR LEFT AND RIGHT LINES

std::vector<cv::Point>
birdsEye::regression(std::vector<std::vector<cv::Vec4i>> left_right_lines,
                     cv::Mat inputImage) {
  std::vector<cv::Point> output(4);
  cv::Point ini;
  cv::Point fini;
  cv::Point ini2;
  cv::Point fini2;
  cv::Vec4d right_line;
  cv::Vec4d left_line;
  std::vector<cv::Point> right_pts;
  std::vector<cv::Point> left_pts;

  // If right lines are being detected, fit a line using all the init and final
  // points of the lines
  if (right_flag == true) {
    for (auto i : left_right_lines[0]) {
      ini = cv::Point(i[0], i[1]);
      fini = cv::Point(i[2], i[3]);

      right_pts.push_back(ini);
      right_pts.push_back(fini);
    }

    if (right_pts.size() > 0) {
      // The right line is formed here
      cv::fitLine(right_pts, right_line, CV_DIST_L2, 0, 0.01, 0.01);
      right_m = right_line[1] / right_line[0];
      right_b = cv::Point(right_line[2], right_line[3]);
    }
  }

  // If left lines are being detected, fit a line using all the init and final
  // points of the lines
  if (left_flag == true) {
    for (auto j : left_right_lines[1]) {
      ini2 = cv::Point(j[0], j[1]);
      fini2 = cv::Point(j[2], j[3]);

      left_pts.push_back(ini2);
      left_pts.push_back(fini2);
    }

    if (left_pts.size() > 0) {
      // The left line is formed here
      cv::fitLine(left_pts, left_line, CV_DIST_L2, 0, 0.01, 0.01);
      left_m = left_line[1] / left_line[0];
      left_b = cv::Point(left_line[2], left_line[3]);
    }
  }

  // One the slope and offset points have been obtained, apply the line equation
  // to obtain the line points
  int ini_y = inputImage.rows;
  int fin_y = 470;

  double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
  double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;

  double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
  double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;

  output[0] = cv::Point(right_ini_x, ini_y);
  output[1] = cv::Point(right_fin_x, fin_y);
  output[2] = cv::Point(left_ini_x, ini_y);
  output[3] = cv::Point(left_fin_x, fin_y);

  return output;
}

std::string birdsEye::predictTurn() {
  std::string output;
  double vanish_x;
  double thr_vp = 10;

  // The vanishing point is the point where both lane boundary lines intersect
  vanish_x = static_cast<double>(
      ((right_m * right_b.x) - (left_m * left_b.x) - right_b.y + left_b.y) /
      (right_m - left_m));

  // The vanishing points location determines where is the road turning
  if (vanish_x < (img_center - thr_vp))
    output = "LEFT OF PLATE";
  else if (vanish_x > (img_center + thr_vp))
    output = "RIGHT OF PLATE";
  else if (vanish_x >= (img_center - thr_vp) &&
           vanish_x <= (img_center + thr_vp))
    output = "GO STRAIGHT";

  return output;
}

cv::Mat birdsEye::plotLane(cv::Mat inputImage, std::vector<cv::Point> lane,
                           std::string turn) {
  std::vector<cv::Point> poly_points;
  cv::Mat output;

  // Create the transparent polygon for a better visualization of the lane
  inputImage.copyTo(output);
  poly_points.push_back(lane[2]);
  poly_points.push_back(lane[0]);
  poly_points.push_back(lane[1]);
  poly_points.push_back(lane[3]);
  cv::fillConvexPoly(output, poly_points, cv::Scalar(0, 0, 255), CV_AA, 0);

  cv::addWeighted(output, 0.3, inputImage, 1.0 - 0.3, 0, inputImage);

  // Plot both lines of the lane boundary
  cv::line(inputImage, lane[0], lane[1], cv::Scalar(0, 255, 255), 5, CV_AA);
  cv::line(inputImage, lane[2], lane[3], cv::Scalar(0, 255, 255), 5, CV_AA);

  // Plot the turn message
  cv::putText(inputImage, turn, cv::Point(50, 90),
              cv::FONT_HERSHEY_COMPLEX_SMALL, 3, cvScalar(0, 255, 0), 1, CV_AA);

  return inputImage;

  // Show the final output image
  // cv::namedWindow("Lane", CV_WINDOW_AUTOSIZE);
  // cv::imshow("Lane", inputImage);

  // cv::waitKey(100);
}
