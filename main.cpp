/**
  @file main.cpp
  @brief An example of usage of Camera class: calibration and normal run
  @author Michal Labowski
  @date 22-12-2019
  @version 1.0
 */

#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include "camera.h"

//#define RUN_TESTS
//#define CAM_CALIBRATE
int main(int argc, char** argv)
{
#ifndef RUN_TESTS
    camera_ns::Camera cam;
    cam.set_video_source(0);
    cam.set_camera_calibration_results_file_name("cam_calib.txt");
#ifdef CAM_CALIBRATE
    cam.set_chessboard_dimensions(6,9);
    cam.set_chessboard_square_dimension(0.0268f);
    cam.set_number_of_images_to_calibrate(15);
    try {
        cam.calibrate();
    } catch (camera_ns::ExceptionMessage ex) {
        std::cout << ex.msg << std::endl;
    }
#endif
    if(cam.get_calibrated() == false) {
        try {
            cam.load_camera_calibration_data();
        } catch (camera_ns::ExceptionMessage ex) {
            std::cout << ex.msg << std::endl;
        }
    }
    while (true) {
        try {
            cam.read();
            cam.show_frame_raw();
            cam.compensate_distortions(camera_ns::CorrectionType::undistort);
            cam.show_frame_compensated();
        } catch (camera_ns::ExceptionMessage ex) {
            std::cout << ex.msg << std::endl;
        }
        cv::waitKey(10);
    }
#else
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
}
