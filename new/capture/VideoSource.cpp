#include "VideoSource.hpp"

VideoSource::VideoSource() {
  std::cout << "Construtor Padrão de VideoSource" << std::endl;
}

VideoSource::VideoSource(int height, int width) {
  camSize.height = height;
  camSize.width = width;

  leftImg = cv::Mat(camSize, CV_8UC3);
  rightImg = cv::Mat(camSize, CV_8UC3);

  thread_cap_left = std::thread(&VideoSource::function_cap_left, this);
  thread_cap_right = std::thread(&VideoSource::function_cap_right, this);
}

VideoSource::~VideoSource(){
  thread_cap_left.join();
  thread_cap_right.join();
}

void VideoSource::function_cap_left() {
  cv::VideoCapture leftCam;
  cv::Mat tempImg;
  leftCam.open(n_left);
  leftCam.set(CV_CAP_PROP_FPS,15);

  try {
    if (!leftCam.isOpened()) {
      throw std::runtime_error(std::string(" Couldn't open the left camera"));
    }
  } catch (std::exception &e) {
    std::cout << e.what();
    exit(0);
  }

  while (leftCam.grab()) {
    mutex_left.lock();
    std::cout<<"left"<<std::endl;

    leftCam.retrieve(tempImg, CV_8UC3);
    leftImg = tempImg;
    mutex_left.unlock();
  }
}

void VideoSource::function_cap_right() {
  cv::VideoCapture rightCam;
  cv::Mat tempImg;
  rightCam.open(n_right);
  rightCam.set(CV_CAP_PROP_FPS,15);

  try {
    if (!rightCam.isOpened()) {
      throw std::runtime_error(std::string(" Couldn't open the right camera"));
    }
  } catch (std::exception &e) {
    std::cout << e.what();
    exit(0);
  }

  while (rightCam.isOpened()) {
    std::cout<<"right"<<std::endl;
    mutex_right.lock();
    rightCam.read(tempImg);
    rightImg = tempImg;
    mutex_right.unlock();
  }
}

void VideoSource::grabRgbBw(cv::Mat &imBwL, cv::Mat &imBwR, cv::Mat &imRgbL, cv::Mat &imRgbR){
  // mutex_left.lock();
  // mutex_right.lock();
  imRgbL = leftImg;
  imRgbR = rightImg;
  // mutex_left.unlock();
  // mutex_right.unlock();

  cv::cvtColor(imRgbL, imBwL, cv::COLOR_RGB2GRAY);
	cv::cvtColor(imRgbR, imBwR, cv::COLOR_RGB2GRAY);
}
